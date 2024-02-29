#include <Arduino.h>
#include <ArduinoHA.h>
#include <arduino_secrets.h> // contains secret credentials and API keys for Arduino project.
#include <WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <DFRobot_SHT3x.h>
#include "DFRobot_VEML7700.h"
#include <ha_functions.h>
#include <ha_config.h>

// https://dronebotworkshop.com/soil-moisture/
#define SENSOR_ONE_PIN A0   // ESP32 pin GPIO36 (ADC0) that connects to AOUT pin of moisture sensor
#define SENSOR_TWO_PIN A1   //  A1-GPIO39
#define SENSOR_THREE_PIN A2 // A2-GPIO34


// The number of read loops since we started which is doen every THRESHOLD
int16_t readCount = 0;
// This it the last time the s`ensors were updated
unsigned long lastUpdateAt = 0;
unsigned long startupTime = millis();

char ssid[] = SECRET_SSID; // your network SSID (name)
char pass[] = SECRET_PASS; // your network password (use for WPA, or use as key for WEP)
char mqttUser[] = MQTT_HA_BROKER_USERNAME;
char mqttUserPass[] = MQTT_HA_BROKER_PASSWORD;
char deviceName[] = "GrowTent";
byte myId[] = {111, 111, 111, 111, 111, 111}; // mac address of the device
/*
 * Instantiate an objects to drive the sensors
 */
// DFRobot_SHT3x sht3x(&Wire,/*address=*/0x45,/*RST=*/4);
DFRobot_SHT3x sht3x;
DFRobot_VEML7700 als;

HADevice device(myId, sizeof(myId));

WiFiClient wifiClient;

// assign the device and the sensor to the MQTT client and make the max number of sensors
HAMqtt mqtt(wifiClient, device, MQTT_SENSOR_COUNT);

const int numReadings = 1000;

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

// ==================== SENSOR SENSOR DEFINITiON ====================
// A sensor is a prt of this device that measures a physical quantity and converts it into a signal
// The unique ID of the sensor. It needs to be unique in a scope of your device.
HASensorNumber uptimeSensor("GT_uptime"); // "ardUptime"
// "myAnalogInput" is unique ID of the sensor. You should define your own ID. (PrecisionP2 is points after the decimal point)
HASensorNumber moisture1("Moisture_1", HASensorNumber::PrecisionP1);
HASensorNumber moisture2("Moisture_2", HASensorNumber::PrecisionP1);
HASensorNumber moisture3("Moisture_3", HASensorNumber::PrecisionP1);
HASensorNumber moisture4("Moisture_4", HASensorNumber::PrecisionP1);
HASensorNumber rh("RH", HASensorNumber::PrecisionP1);
HASensorNumber aTemp("Ambient_Temperature", HASensorNumber::PrecisionP1);
HASensorNumber lux("Lux", HASensorNumber::PrecisionP1);

/**
 * setup() is called once at the start of the program
 */
void setup()
{
  // DEBUG_INIT();
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println("Starting setup...");
  // Serial.println("DNS and DHCP-based web client test 2024-02-04"); // so I can keep track of what is loaded start the Ethernet connection:connect to wifi

  // ==================== Setup Action Pins ====================
  // WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500); // waiting for the connection
  }
  Serial.println();
  Serial.println("Connected to the network");
  // printCurrentNet(WiFi.SSID(), bssid, myId, WiFi.RSSI(), WiFi.encryptionType());

  /**
   * softReset Send command resets via IIC, enter the chip's default mode single-measure mode,
   * turn off the heater, and clear the alert of the ALERT pin.
   * @return Read the register status to determine whether the command was executed successfully,
   * and return true indicates success.
   */
  if (!sht3x.softReset())
  {
    Serial.println("Failed to Initialize the chip....");
  }
  // Enable the heater so moiture does not condense on the sensor
  sht3x.heaterEnable();
  // set device's details (optional)
  device.setSoftwareVersion("1.0.0");
  device.setManufacturer("dfrobot");
  device.setModel("firebeetle2_esp32e");
  device.setName(deviceName);
  // ...
  device.enableSharedAvailability();
  // device.setAvailability(false); // changes default state to offline
  //  MQTT LWT (Last Will and Testament) is a feature of the MQTT protocol that allows a client to notify the broker of an ungracefully disconnected client.
  device.enableLastWill();
  // Discovery prefix is used to build the MQTT topic for the discovery messages.
  mqtt.setDiscoveryPrefix("homeassistant"); // this is the default value
  mqtt.setDataPrefix("aha");                // this is the default value

  // configure sensor (optional)
  uptimeSensor.setIcon("mdi:timer");
  uptimeSensor.setName("Uptime");
  uptimeSensor.setUnitOfMeasurement("s");
  //
  moisture1.setIcon("mdi:water-percent");
  moisture1.setName("Moisture Plant 1");
  moisture1.setUnitOfMeasurement("%");
  //
  moisture2.setIcon("mdi:water-percent");
  moisture2.setName("Moisture Plant 2");
  moisture2.setUnitOfMeasurement("%");
  //
  moisture3.setIcon("mdi:water-percent");
  moisture3.setName("Moisture Plant 3");
  moisture3.setUnitOfMeasurement("%");
  //
  moisture4.setIcon("mdi:water-percent");
  moisture4.setName("Moisture Plant 4");
  moisture4.setUnitOfMeasurement("%");
  //
  rh.setIcon("mdi:water-percent");
  rh.setName("Relative Humidity");
  rh.setUnitOfMeasurement("%");
  //
  aTemp.setIcon("mdi:thermometer");
  aTemp.setName("Ambient Temperature");
  aTemp.setUnitOfMeasurement("°C");
  //
  lux.setIcon("mdi:weather-sunny");
  lux.setName("Lux");
  lux.setUnitOfMeasurement("lx");
  // start the mqtt broker connection

  // mqtt.begin(BROKER_ADDR, mqttUser, mqttUserPass);
  mqtt.begin(BROKER_ADDR);
}

/**
 * loop() is called repeatedly
 */
void loop()
{
  mqtt.loop();
  // Update Read the sensors every THRESHOLD
  if (millis() - lastUpdateAt >= THRESHOLD)
  {
    DEBUG_PRINT("Updating sensor value for uptimeSensor: ");
    readCount++;
    DEBUG_PRINTLN(readCount);
    //uptimeSensor.setValue(readCount);
    uint32_t uptimeValue = millis() / 1000;
    uptimeSensor.setValue(uptimeValue);
    // ignore the imitial readings as it takes time for the sensors to stabilize
    if (readCount > INITIAL_READER_COUNTER)
    {
      // read from the sensor moisture1
      int soilMoistureValue = analogRead(SENSOR_ONE_PIN); // read the analog value from sensor
      DEBUG_PRINT("Updating moisture1 sensor value: ");
      DEBUG_PRINTLN(soilMoistureValue);
      moisture1.setValue(getPercentage(soilMoistureValue));
      // read from the sensor moisture2
      int soilMoistureValue2 = analogRead(SENSOR_TWO_PIN); // read the analog value from sensor
      DEBUG_PRINT("Updating moisture2 sensor value: ");
      DEBUG_PRINTLN(soilMoistureValue2);
      moisture2.setValue(getPercentage(soilMoistureValue2));
      // read from the sensor moisture3
      int soilMoistureValue3 = analogRead(SENSOR_THREE_PIN); // read the analog value from sensor
      DEBUG_PRINT("Updating moisture3 sensor value: ");
      DEBUG_PRINTLN(soilMoistureValue3);
      moisture3.setValue(getPercentage(soilMoistureValue3));
      // read from the sensor moisture4
      int soilMoistureValue4 = analogRead(SENSOR_THREE_PIN); // read the analog value from sensor
      DEBUG_PRINT("Updating moisture4 sensor value: ");
      DEBUG_PRINTLN(soilMoistureValue4);
      moisture4.setValue(getPercentage(soilMoistureValue4));
      // read from the relative humidity sensor
      rh.setValue(sht3x.getHumidityRH());
      DEBUG_PRINT("Rleative Humidity sensor value: ");
      DEBUG_PRINTLN(sht3x.getHumidityRH());

      // read from the temperature sensor
      aTemp.setValue(sht3x.getTemperatureC());
      DEBUG_PRINT("Ambient Temperature sensor value: ");
      DEBUG_PRINTLN(sht3x.getHumidityRH());

      // read from the light sensor
      float luxValue;
      als.getALSLux(luxValue);
      lux.setValue(luxValue);
      DEBUG_PRINT("Lux sensor value: ");
      DEBUG_PRINTLN(luxValue);
      // reset loop timer
      lastUpdateAt = millis();
    } // end of if readCount
  }   // end of if millis
} // end of loop
