#define SERIAL_BAUD 115200
#define GPS_BAUD 9600
#define TIMEZONE_UTC -7
#define SCAN_INTERVAL 500



/* Pin Definitions */
#define SD_CS  D8

#if defined(ESP8266)
    #define GPS_RX 5
    #define GPS_TX 16
#elif defined(ESP32)
    #define SDA 33
    #define SCL 35
    #define GPS_RX D4
    #define GPS_TX D3
#endif