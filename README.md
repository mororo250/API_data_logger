v# Data Logger Shield para Arduino com RTC DS1307

# Projeto: 

Esse projeto foi desenvolvido no contexto da disciplina “Programação de Sistemas Embarcados” ofertada para o curso de Engenharia Elétrica da UFMG. O objetivo desse projeto é desenvolver uma API para o Data Logger Shield.

## Hardware: 

O data logger utilizado é o data logger shield da adafruit. Ele possui ... 


Foi utilizado o arduino uno. E os sensores de temperatura L35 e o sensor de humidade DH22. També foi utilizado o software proteus para simular os projeto.


# API

A API desenvolvida permite utilziar o data logger de forma rapida e facíl, mantendo um grande nivel de flexibilidade.

## Variables:


Pin Variables.

```C
int         m_redLEDpin;
int         m_greenLEDpin;
int         m_SDpin;
```

```C
int         m_sync_interval;
int         m_log_interval;
```
```C
uint8_t     m_flags;  
```
```C
File        m_logfile;
```
```C
char*       m_filename; 
```
```C
RTC_DS1307  m_RTC;
```

## Funções

void setup();
void create_file(char const* filename);      // Replace file name if filename exists.
void open_file(char const* filename);        // Open existing file. 
void log_header(char const* str);
void log_time();

void record()
void run()
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



## Código exemplo 



