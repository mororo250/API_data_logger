#include <DataLogger.h>
#include <DHT.h>


float temp;
int tempPin = 0;
int humitypin = 4;

DataLogger dt;

DHT dht(humitypin, DHT22);

void setup() 
{
  Serial.begin(9600);
  dht.begin();

  dt.set_green_Led_pin(3);
  dt.set_red_Led_pin(2);
  dt.set_sync_interval(10000);
  dt.set_log_interval(2000);
  dt.setup();

  dt.create_file("Test.csv");
  dt.log_header("Temperature, Humidity");

}

void loop() 
{
  temp = analogRead(tempPin);

  temp = temp * 0.48828125;  // convert the analog volt to its temperature equivalent
  float h = dht.readHumidity();

  dt.run(temp, h);
  delay(1000);
}
