#include <Arduino.h>

// https://dronebotworkshop.com/soil-moisture/
#define SENSOR_ONE_PIN A0 // ESP32 pin GPIO36 (ADC0) that connects to AOUT pin of moisture sensor
#define SENSOR_TWO_PIN A1 //  A1-GPIO39
#define SENSOR_THREE_PIN A2 // A2-GPIO34
#define THRESHOLD 1000 // CHANGE YOUR THRESHOLD HERE

const int numReadings = 1000;

const int AIR_VALUE = 3000;  // you need to replace this value with 2500 
const int WATER_VALUE = 761; // you need to replace this value with 770 (0.93863 v)

int readings[numReadings]; // the readings from the analog input
int readIndex = 0;         // the index of the current reading
int totalReads = 1;
int total = 0;   // the running total
int average = 0; // the average
int intervals = (AIR_VALUE - WATER_VALUE) / 3;
long lastmillis;
int lowestValue = 100000;
int higestValue = 0;
// analogReadResolution(12); // The Zero, Due, MKR family and Nano 33 (BLE and IoT) boards have 12-bit ADC capabilities

int getPercentage(int value)
{
  // Determine soil moisture percentage value
  value = map(value, AIR_VALUE, WATER_VALUE, 0, 100);
  // Keep values between 0 and 100
  value = constrain(value, 0, 100);
  return value;
}

void setup()
{
  Serial.begin(115200); // open serial port, set the baud rate as 9600 bps
  Serial.print("intervals: ");
  Serial.println(intervals);
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
  {
    readings[thisReading] = 0;
  }
}

void loop()
{
  if (millis() - lastmillis >= THRESHOLD) // Update every THRESHOLD
  {
    Serial.print("(");
    Serial.print(readIndex);
    Serial.println(") ");
    // subtract the last reading:
    total = total - readings[readIndex];
    int soilMoistureValue = analogRead(SENSOR_ONE_PIN); // read the analog value from sensor
    Serial.print("   Rosemary soilMoistureValue: ");
    Serial.print(soilMoistureValue);
    Serial.print(" soilMoisturePercentage: ");
    Serial.println(getPercentage(soilMoistureValue));
    readings[readIndex] = soilMoistureValue;
    // Second or A1
    int soilMoistureValue2 = analogRead(SENSOR_TWO_PIN); // read the analog value from sensor
    Serial.print("   Sage soilMoistureValue: ");
    Serial.print(soilMoistureValue2);
    Serial.print(" soilMoisturePercentage: ");
    Serial.println(getPercentage(soilMoistureValue2));

        // Second or A1
    int soilMoistureValue3 = analogRead(SENSOR_THREE_PIN); // read the analog value from sensor
    Serial.print("   Parsley soilMoistureValue: ");
    Serial.print(soilMoistureValue3);
    Serial.print(" soilMoisturePercentage: ");
    Serial.println(getPercentage(soilMoistureValue3));

    // add the reading to the total:
    total = total + readings[readIndex];
    // advance to the next position in the array:
    readIndex = readIndex + 1;

    // if we're at the end of the array...
    if (readIndex >= numReadings)
    {
      // ...wrap around to the beginning:
      Serial.print("Rosemary average soilMoistureValue: ");
      Serial.println(total / numReadings);
      readIndex = 0;
    }

    // calculate the average:
    average = total / numReadings;
    // delay(1000);
    lastmillis = millis();
  }
}