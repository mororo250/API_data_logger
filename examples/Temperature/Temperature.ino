#include <DataLogger.h>

float temp;
int tempPin = 0;

DataLogger* dt;

void setup() 
{
  Serial.begin(9600);

  delay(1000);
  dt = new DataLogger();
  dt->setup();
  dt->create_file("Test.csv");
  dt->log_header("Temperature");
}

void loop() 
{
  temp = analogRead(tempPin);

  temp = temp * 0.48828125;
  // convert the analog volt to its temperature equivalent

  dt->run(temp);
  delay(1000);
}
