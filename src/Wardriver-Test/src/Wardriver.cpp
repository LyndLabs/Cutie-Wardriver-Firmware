#include "Arduino.h"
#include "Wardriver.h"
#include "Recon.h"
#include "Screen.h"
#include <SD.h>

#define SD_CS  D8
char filename[23];
File file;

float lat = 0; float lng = 0;

// CURRENT DATETIME
uint8_t hr; uint8_t mn; uint8_t sc;
uint8_t yr; uint8_t mt; uint8_t dy;

Wardriver::Wardriver() {

}

void initGPS() {

}

void initGPS(uint8_t override) {
    yr = 2023;
    mn = 7;
    dy = 14;
}

void initFS() {
    uint8_t logNum = 0;
    char * wiglePreHeader = "WigleWifi-1.4,appRelease=1.0,model=DevKit,release=1.0,device=WiFi Nugget,display=SH1106,board= ESP32-S2,brand=LyndLabs";
    char * wigleHeader    = "MAC,SSID,AuthMode,FirstSeen,Channel,RSSI,CurrentLatitude,CurrentLongitude,AltitudeMeters,AccuracyMeters,Type";
    
    while (true) {
        sprintf(filename,"/NuggWD-%i%.02i%.02i-%i.csv",yr, mn, dy, logNum);
        // if (!FFat.exists(filename)) { break; }
        if (SD.exists(filename)) { break; }
        logNum++;
    }

    // file = FFat.open(String (filename), FILE_WRITE);
    file = SD.open(filename, FILE_WRITE);

    file.println(wiglePreHeader);
    file.println(wigleHeader);
    file.close();

}

void Wardriver::init() {
    Screen::init();
    Screen::drawSplash(2);

    initGPS(0);
    initFS();
}

