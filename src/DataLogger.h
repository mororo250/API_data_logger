#ifndef MY_LIBRARY_H
#define MY_LIBRARY_H

#include "Arduino.h"
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <RTClib.h>

#ifdef DEBUG_ON
#define DEBUG(x) Serial.print(x)
#define DEBUGLN(x) Serial.println(x)
#else
#define DEBUG(x) do {} while (0)
#define DEBUGLN(x)  do {} while (0)
#endif

#define LED_BLINK_TIME 50

class DataLogger
{
  public:

  DataLogger(int const redLedpin = 2, int const greenLedpin = 3, int const Sdpin = 10, int const sync_interval = 10000, int const log_interval = 1000);

  void setup();
  void create_file(char const* filename);      // Replace file name if filename exists.
  void open_file(char const* filename);        // Open existing file. 
  void log_header(char const* str);
  void log_time();

  void record(char const* src) { m_logfile.print(src); }

  template<typename T>
  void record(T const& value)
  {
    m_logfile.print(value);
    m_logfile.print(", ");
  }  

  template<typename T, class ... Types>
  void record(T const& value, Types const& ... args)
  {
    m_logfile.print(value);
    m_logfile.print(", ");

    record(args...);
  }

  void record(){} // if there isn't a argument do nothing.

  template<class ... Types>
  void run(Types const& ... args)
  {    
    static uint32_t s_log_time = 0;

    if ((millis() - s_log_time) < m_log_interval) return;
    s_log_time = millis();

    if (!m_logfile && (m_flags & FLAG_SLEEP_MODE)) 
    {
      error("No file is open.");
    }

    record(args ...);
    if(m_flags & FLAG_LOG_DATE_TIME)
      log_time();

    if(m_flags & FLAG_WRITING_LED)
      writing_LED();

    m_logfile.print("\n"); // print end line.
    
    DEBUGLN("Write to file.");

    sync();
  }

  // Now we write data to disk! Don't sync too often - requires 2048 bytes of I/O to SD card  
  void sync(); // which uses a bunch of power and takes time

  void set_red_Led_pin(int const redLedpin) { m_redLEDpin = redLedpin;}
  void set_green_Led_pin(int const greenLedpin) {m_greenLEDpin = greenLedpin;}
  void set_SD_pin(int const SDpin) {m_SDpin = SDpin;}
  void set_sync_interval(int const sync_interval) {m_sync_interval = sync_interval;}
  void set_log_interval(int const log_interval) {m_log_interval = log_interval;}
  void set_time(uint16_t year, uint8_t month, uint8_t day, uint8_t hour = 0, uint8_t min = 0, uint8_t sec = 0);


  int get_red_Led_pin() const {return m_redLEDpin;} 
  int get_green_Led_pin() const {return m_greenLEDpin;}
  int get_SD_pin() const {return m_SDpin;}
  int get_sync_interval() {return m_sync_interval;}
  int get_log_interval() {return m_log_interval;}
  RTC_DS1307 get_real_time_clock() {return m_RTC;}

  void writing_LED();
  void syncing_LED();

  void enable_writingLED() { m_flags |= FLAG_WRITING_LED; }
  void enable_syncingLED() { m_flags |= FLAG_SYNCING_LED; }
  void enable_time_log() { m_flags |= FLAG_LOG_DATE_TIME; }
  void sleep() { m_flags |= FLAG_SLEEP_MODE;  digitalWrite(m_SDpin, LOW); }

  void disable_writingLED() { m_flags &= ~FLAG_WRITING_LED; }
  void disable_syncingLED() { m_flags &= ~FLAG_SYNCING_LED; }
  void disable_time_log() { m_flags &= ~FLAG_LOG_DATE_TIME; }
  void wake_up() { m_flags &= ~FLAG_SLEEP_MODE; digitalWrite(m_SDpin, HIGH); }

  private:

  enum LOGGER_FLAGS
  {
    FLAG_WRITING_LED    = 1,          // LED informs whens is writing.              - True - Default
    FLAG_SYNCING_LED    = 1 << 1,     // LED informs when is syncing to disc.       - True - Default   
    FLAG_LOG_DATE_TIME  = 1 << 2,     // Logs time and date during run().           - True - Default
    FLAG_SLEEP_MODE     = 1 << 3,     
  } ;

  int         m_redLEDpin;
  int         m_greenLEDpin;
  int         m_SDpin;

  int         m_sync_interval;
  int         m_log_interval;
  uint8_t     m_flags;  

  File        m_logfile;
  RTC_DS1307  m_RTC;
  char*       m_filename; 

  void error(char *str);
  void LEDsuccess();
  };

#endif
