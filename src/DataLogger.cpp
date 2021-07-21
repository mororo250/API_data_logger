#include "DataLogger.h"

DataLogger::DataLogger(int const redLEDpin, int const greenLEDpin, int const Sdpin, int const sync_interval, int const log_interval)
: m_redLEDpin(redLEDpin)
, m_greenLEDpin(greenLEDpin)
, m_SDpin(Sdpin)
, m_sync_interval(sync_interval)
, m_log_interval(log_interval)
, m_filename("ABCDE.csv")
, m_flags(FLAG_WRITING_LED |  FLAG_SYNCING_LED | FLAG_LOG_DATE_TIME)
{
}

void DataLogger::setup()
{  
  // use debugging LEDs
  pinMode(m_redLEDpin, OUTPUT);
  pinMode(m_greenLEDpin, OUTPUT);

  // initialize the SD card
  Serial.println("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(m_SDpin, OUTPUT);

  if (!SD.begin(m_SDpin))
    error("Card failed, or not present");
  Serial.println("card initialized.");

  Wire.begin();  
  if (!m_RTC.begin())
    error("RTC failed");

  LEDsuccess();
}

void DataLogger::create_file(char const *filename)
{
  strcpy(m_filename, filename);

  while(true)
  {  
    if (! SD.exists(m_filename)) 
    {
      // only open a new file if it doesn't exist
      m_logfile = SD.open(m_filename, FILE_WRITE);
      break;
    }
    else
    {
    SD.remove(m_filename);
    Serial.print("Deleting Curent ");
    Serial.print(m_filename);
    Serial.println(" Content");
    m_logfile = SD.open(m_filename, FILE_WRITE);     
    }
  }
  if (!m_logfile) 
  {
    error("couldnt create file");
  }
}

void DataLogger::open_file(char const* str)
{
  if (SD.exists(m_filename)) 
    m_logfile = SD.open(m_filename, FILE_WRITE);
  else
    error("The file does not exist.");
}      

void DataLogger::log_header(char const* str) // Todo: make sure to alweys write in the first line.
{
  // Set number of Variables 
  m_logfile.print(str);
  if(m_flags & FLAG_LOG_DATE_TIME)
  {
    m_logfile.print(",");
    m_logfile.print("millis,datetime");
  }
  m_logfile.print("\n"); 
}


void DataLogger::log_time()
{
  DateTime now;

  // log milliseconds since starting
  uint32_t m = millis();
  m_logfile.print(m);       // milliseconds since start
  m_logfile.print(", ");
  
  // fetch the time
  now = m_RTC.now();
  
  m_logfile.print(", ");
  m_logfile.print('"');
  m_logfile.print(now.year(), DEC);
  m_logfile.print("/");
  m_logfile.print(now.month(), DEC);
  m_logfile.print("/");
  m_logfile.print(now.day(), DEC);
  m_logfile.print("  ");
  m_logfile.print(now.hour(), DEC);
  m_logfile.print(":");
  m_logfile.print(now.minute(), DEC);
  m_logfile.print(":");
  m_logfile.print(now.second(), DEC);
  m_logfile.print('"');
}

void DataLogger::sync()
{
  static uint32_t s_sync_time = 0;

  if ((millis() - s_sync_time) < m_sync_interval) return;
  s_sync_time = millis();
    
  m_logfile.flush();
    
  // blink LED to show we are syncing data to the card & updating FAT!
  if(m_flags & FLAG_SYNCING_LED)
    syncing_LED();

  DEBUGLN("Sync with disk.");   
}

void DataLogger::set_time(uint16_t year, uint8_t month, uint8_t day, uint8_t hour = 0, uint8_t min = 0, uint8_t sec = 0)
{
  m_RTC.adjust(DateTime(year, month, day, hour, min, sec));
}

void DataLogger::error(char *str)
{
  Serial.print("error: ");
  Serial.println(str);
  
  // red LED indicates error
  digitalWrite(m_redLEDpin, HIGH);
  while(true);
}

void DataLogger::LEDsuccess()
{
  // Peppy blinky pattern to show that the logger has successfully initialized
  digitalWrite(m_greenLEDpin,HIGH);
  delay(500);
  digitalWrite(m_greenLEDpin,LOW);
  delay(250);
  digitalWrite(m_greenLEDpin,HIGH);
  delay(100);
  digitalWrite(m_greenLEDpin,LOW);
  delay(100);
  digitalWrite(m_greenLEDpin,HIGH);
  delay(100);
  digitalWrite(m_greenLEDpin,LOW);
}

void DataLogger::writing_LED()
{
  digitalWrite(m_greenLEDpin, HIGH);
  delay(LED_BLINK_TIME);
  digitalWrite(m_greenLEDpin, LOW);
}

void DataLogger::syncing_LED()
{
  digitalWrite(m_redLEDpin, HIGH);
  digitalWrite(m_greenLEDpin, HIGH);
  delay(LED_BLINK_TIME);
  digitalWrite(m_redLEDpin, LOW);
  digitalWrite(m_greenLEDpin, LOW);
}
