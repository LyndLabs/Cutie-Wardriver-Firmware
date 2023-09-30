#pragma once
#include "Arduino.h"
#include <HardwareSerial.h>

#define VERSION 1.1
#define VERSION_STR "v1.2"
#define WIGLE_HEADER "MAC,SSID,AuthMode,FirstSeen,Channel,RSSI,CurrentLatitude,CurrentLongitude,AltitudeMeters,AccuracyMeters,Type"

#define SERIAL_BAUD 1152007
#define GPS_BAUD 9600
#define TIMEZONE_UTC -7
#define SCAN_INTERVAL 500

/* Pin Definitions */
#define SD_CS  D8

#if defined(ESP8266)
    #define GPS_RX D4
    #define GPS_TX D3
    #define SERIAL_VAR ss

    #define FS_VAR SD
#elif defined(ESP32)
    #define SDA 33
    #define SCL 35

    #define SERIAL_VAR Serial1
    #define GPS_RX 3  // AO
    #define GPS_TX 16 // D4
    
    #define FS_VAR FFat
#endif


#if defined(ESP8266)
    #define LOG_PREFIX "DNS_Driveby"
    #define DEVICE "DNS Driveby"
    #define MODEL "Developer Kit"
    #define BOARD "ESP8266"
    
#elif defined(ESP32) 
    #define LOG_PREFIX "Nugget"
    #define DEVICE "Nugget"
    #define MODEL "DevKitty"
    #define BOARD "ESP32-S2"
#endif