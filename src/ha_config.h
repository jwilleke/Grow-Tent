/**
 * @file  ha_config.h
 * These are values that are generally specific to this project and the associated hardware.
*/
#pragma once

// ==================== OPERATIONAL SPECIFICS ====================
#define SERIAL_BAUD_RATE 115200

#define DEBUG_PRINT_ENABLED // Comment out this line of code if you don't want to see the debug print


#define INITIAL_READER_COUNTER 200
#define INITIAL_READER_COUNTER 200
#define THRESHOLD 5000      // CHANGE YOUR THRESHOLD HERE

// ==================== MQTT SPECIFICS ====================
#define BROKER_ADDR IPAddress(192, 168, 68, 27) // MQTT broker address  
#define MQTT_BROKER_PORT 1883                    // MQTT broker port

// ==================== DEVICE SPECIFICS ====================
#define ANALOG_SUPPLY_VOLTAGE 5.0
// growtent: 64:b7:08:2e:5d:14
#define MAC_ADDRESS {0x30, 0xAE, 0xA4, 0xB2, 0xC3, 0xD4} // The MAC address of this device
#define DEVICENAME "GrowTent"; // This Host device name
#define PLATFORM "espressif32" // This is the platform name
#define BOARD_MODEL "dfrobot_firebeetle2_esp32e" // This is the board model name
#define HOSTNAME "nutrient-tank.nerdsbythehour.com"
#define FRAMEWORK "arduino" // This is the framework name

// ==================== SENSOR SPECIFICS ====================
#define MQTT_SENSOR_COUNT 12
/*
Hardware ping addreses
Sensor pin settings
*/
#define PHPIN A4  // semsors[0]
#define TDSPIN A1 // semsors[1]
#define DOPIN A5  // not used but is ues in GravityDo,cpp
#define ORPPIN A2 // semsors[2]
#define ECPIN A3  // semsors[3]
#define ONEWIREPIN 2 // The Digital I/O pin number for the one wire bus
#define LEVELPIN 4

// Digital I/O pin settings for the pumps fans and valves
//Relay 1
#define PHUP_SOLUTION_PUMP 8
#define PHDOWN_SOLUTION_PUMP 9
//Relay 2
#define A1SOLUTION_PUMP 10
#define A2SOLUTION_PUMP 11
//Relay 3 (Two STIRERs on each set of relay contacts)
#define SOLUTION_STIRER_1 12
#define SOLUTION_STIRER_2 13

#define PUMP_RUNTIME 20000

// EC sesor values
#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}

#define KVALUEADDR 0x0A    //the start address of the K value stored in the EEPROM
#define ECRES2 820.0
#define ECREF 200.0


const int AIR_VALUE = 3000;  // you need to replace this value with 2500
const int WATER_VALUE = 761; // you need to replace this value with 770 (0.93863 v)

// ==================== DEBUG SPECIFICS ====================

#if defined(DEBUG_PRINT_ENABLED)
  #define DEBUG_INIT() Serial.begin(SERIAL_BAUD_RATE);
  #define DEBUG_PRINT(x) Serial.print(x);
  #define DEBUG_PRINTLN(x) Serial.println(x);
#else
  #define DEBUG_INIT()
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINT(x)
#endif


