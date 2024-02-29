#pragma once
#include <Arduino.h>
#include <ha_config.h>


/**
 * @brief Print the MAC address of the given byte array
 * 
 * @param mac The byte array to print
 * @param len The length of the byte array
 
*/
void printByetArray(byte mac[], int len)
{
  for (int i = len; i > 0; i--)
  {
    if (mac[i] < 16)
    {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 1)
    {
      Serial.print(":");
    }
  }
  Serial.println();
}

/**
 * @brief Callback function for the switch state change
*/
int getPercentage(int value)
{
  // Determine soil moisture percentage value
  value = map(value, AIR_VALUE, WATER_VALUE, 0, 100);
  // Keep values between 0 and 100
  value = constrain(value, 0, 100);
  return value;
}


/**
 * @brief Print the MAC address of the given byte array
 * 
*/
void printCurrentNet(byte SSID[], byte BSSID[], byte myId[], long rssi, long encryption)
{
  // print the SSID of the network you're attached to:
  DEBUG_PRINT("SSID: ");
  printByetArray(SSID, sizeof(SSID));
  
  // print the MAC address of SSID you're attached to:
  DEBUG_PRINT("BSSID: ");
  printByetArray(BSSID, sizeof(BSSID));

  DEBUG_PRINT("MAC: ");
  printByetArray(myId, sizeof(myId));

  // print the received signal strength:
  DEBUG_PRINT("signal strength (RSSI): ");
  DEBUG_PRINTLN(rssi);

  // print the encryption type:
  DEBUG_PRINT("Encryption Type: ");
  DEBUG_PRINTLN(encryption);
  DEBUG_PRINTLN();
} // end printCurrentNet