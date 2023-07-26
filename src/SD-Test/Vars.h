#pragma once
#include "Arduino.h"
#include <HardwareSerial.h>

#define SERIAL_BAUD 115200
#define GPS_BAUD 9600
#define TIMEZONE_UTC -7
#define SCAN_INTERVAL 500



/* Pin Definitions */
#define SD_CS  D8

#if defined(ESP8266)
    #define GPS_RX D4
    #define GPS_TX D3
    #define SERIAL_VAR ss
#elif defined(ESP32)
    #define SDA 33
    #define SCL 35

    #define SERIAL_VAR Serial1
    #define GPS_RX 5
    #define GPS_TX 6
    
#endif