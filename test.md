#include <Arduino.h>

#define AOUT_PIN A0    // ESP32 pin GPIO36 (ADC0) that connects to AOUT pin of moisture sensor
#define THRESHOLD 1000 // CHANGE YOUR THRESHOLD HERE

const int AirValue = 2600;  // you need to replace this value with Value_1
const int WaterValue = 875; // you need to replace this value with Value_2
int intervals = (AirValue - WaterValue) / 3;
int soilMoistureValue = 0;
float soilMoisturePercentage = 0;
long lastmillis;
int lowestValue = WaterValue;

void setup()
{
  Serial.begin(115200); // open serial port, set the baud rate as 9600 bps
  Serial.print("intervals: ");
  Serial.println(intervals);
}

void loop()
{
  if (millis() - lastmillis >= THRESHOLD) // Update every THRESHOLD
  {
    // Serial.println("in LOOP");
    soilMoistureValue = analogRead(0); // read the analog value from sensor
    soilMoisturePercentage = map(soilMoistureValue, AirValue, lowestValue, 0, 100);
    if (soilMoisturePercentage >= 100)
    {
      soilMoisturePercentage = 100.00;
    }
    if (soilMoisturePercentage <0 )
    {
      soilMoisturePercentage = 0;
    }
    Serial.print("soilMoistureValue: ");
    Serial.print(soilMoistureValue);
    Serial.print("  soilMoisturePercentage: ");
    Serial.println(soilMoisturePercentage);
    if (WaterValue < soilMoistureValue)
    {
      lowestValue = soilMoistureValue;
      Serial.print("  Adjusting WaterValue " + lowestValue);
      Serial.println(" from: " + WaterValue);
    }
    lastmillis = millis();
  }
}


https://how2electronics.com/interface-capacitive-soil-moisture-sensor-arduino/

