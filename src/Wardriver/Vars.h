#pragma once
#include "Arduino.h"

// SET BOARD BEFORE COMPILING
#define QT_WARDRIVER

#define VERSION 1.3
#define VERSION_STR "v1.3"
#define WIGLE_HEADER "MAC,SSID,AuthMode,FirstSeen,Channel,RSSI,CurrentLatitude,CurrentLongitude,AltitudeMeters,AccuracyMeters,Type"

#define SERIAL_BAUD 115200
#define GPS_BAUD 9600
#define TIMEZONE_UTC -7
#define SCAN_INTERVAL 500

/* Pin Definitions */

#if CONFIG_IDF_TARGET_ESP8266
    #warning "Compiling for ESP8266"
    #define GPS_RX D4
    #define GPS_TX D3
    #define SERIAL_VAR ss

    #define FS_VAR SD
#elif CONFIG_IDF_TARGET_ESP32C3
    #define SDA SDA
    #define SCL SCL

    #define SERIAL_VAR Serial1
    #define GPS_RX 20 
    #define GPS_TX 21
    
    #define FS_VAR SD
    #define SD_CS  -1
#elif CONFIG_IDF_TARGET_ESP32S2
    #warning "Compiling for ESP32-S2"
#elif CONFIG_IDF_TARGET_ESP32S3
    #warning "Compiling for ESP32-S3"
#elif CONFIG_IDF_TARGET_ESP32
    #warning "Compiling for ESP32"
#else
  #error "Unrecognized ESP."
#endif

#if defined(DNS_DRIVEBY)
    #define LOG_PREFIX "DNS_Driveby"
    #define DEVICE "DNS Driveby"
    #define MODEL "Developer Kit"
    #define BOARD "ESP8266"

#elif defined(bruh)
    #define LOG_PREFIX "QT-Py"
    #define DEVICE "QT-Py_Wardriver"
    #define MODEL "QT-Py_Wardriver"
    #define BOARD "ESP32-C3"

#elif defined(QT_WARDRIVER) 
    #define LOG_PREFIX "Nugget"
    #define DEVICE "Nugget"
    #define MODEL "DevKitty"
    #define BOARD "ESP32-S2"
#endif