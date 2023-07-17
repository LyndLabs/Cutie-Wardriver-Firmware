#include "Arduino.h"
#include "Wardriver.h"
#include "Recon.h"
#include "Screen.h"
#include <SD.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>


#define SD_CS  D8
char filename[23];
File file;

SoftwareSerial ss(D4, D3); // RX, TX
TinyGPSPlus gps;

// CURRENT GPS & DATTIME STRING
float lat = 0; float lng = 0;
int alt; double hdop;
char strDateTime[15];
char currentGPS[17];

// RECON PARAMS
uint32_t totalNets = 0;
uint8_t  clients = 0;
uint8_t  openNets = 0;
uint8_t  sats = 0;
uint8_t  bat = 0;
uint8_t  speed = 0;

// CURRENT DATETIME
uint8_t hr; uint8_t mn; uint8_t sc;
uint16_t yr; uint8_t mt; uint8_t dy;
char currTime[6];

Wardriver::Wardriver() {

}

static void smartDelay(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

void updateGPS() {
    lat = gps.location.lat(); lng = gps.location.lng();
    alt = (int) gps.altitude.meters();
    hdop = gps.hdop.hdop();
    sats = gps.satellites.value();
    speed = gps.speed.mph();

    yr = gps.date.year();
    mt = gps.date.month();
    dy = gps.date.day();

    hr = gps.time.hour();
    mn = gps.time.minute();
    sc = gps.time.second();

    sprintf(strDateTime,"%i-%i-%i %i:%i:%i",yr,mt,dy,hr,mn,sc);
    sprintf(currentGPS,"%1.3f,%1.3f",lat,lng);
    sprintf(currTime,"%02d:%02d",hr,mn);

    Screen::drawMockup(currentGPS,currTime,sats,totalNets,openNets,clients,bat,speed,"GPS: UPDATED");
}

void updateGPS(uint8_t override) {
    lat = 37.8715, 122.2730; lng = 122.2730;
    alt = 220; hdop = 1.5;
    sats = 3; speed = 69;

    yr = 2023; mt = 7; dy = 14;
    hr = 12; mn = 34; sc = 56;

    sprintf(strDateTime,"%i-%i-%i %i:%i:%i",yr,mt,dy,hr,mn,sc);
    sprintf(currentGPS,"%1.3f,%1.3f",lat,lng);
    sprintf(currTime,"%02d:%02d",hr,mn);

    Screen::drawMockup(currentGPS,currTime,sats,totalNets,openNets,clients,bat,speed,"GPS: UPDATED");
}

// initialize GPS & get first coords
void initGPS() {

  ss.begin(9600);
  if (ss.available() > 0) {
    Screen::drawMockup("...","...",sats,totalNets,openNets,clients,bat,speed,"GPS: Waiting for fix...");
  }
  else {
    Screen::drawMockup("...","...",sats,totalNets,openNets,clients,bat,speed,"GPS: NOT FOUND");
  }
   while (!gps.location.isValid()) {
    sats = gps.satellites.value();
    Screen::drawMockup("...","...",sats,totalNets,openNets,clients,bat,speed,"GPS: Waiting for fix...");
    Serial.println(gps.location.isValid());
    delay(0);
    smartDelay(500);
  }

  updateGPS();
}

void initGPS(uint8_t override) {
    Screen::drawMockup("...","...",0,0,0,0,0,0,"GPS: Waiting for fix");
    delay(500);

    updateGPS(0);
    Screen::drawMockup(currentGPS,currTime,sats,totalNets,openNets,clients,bat,speed,"GPS: LOCATION FOUND");
}

void initFS() {
    uint8_t logNum = 0;
    char * wiglePreHeader = "WigleWifi-1.4,appRelease=1.0,model=DevKit,release=1.0,device=DNS Driveby,display=SH1106,board= ESP8266,brand=LyndLabs";
    char * wigleHeader    = "MAC,SSID,AuthMode,FirstSeen,Channel,RSSI,CurrentLatitude,CurrentLongitude,AltitudeMeters,AccuracyMeters,Type";
   
    if (!SD.begin(SD_CS)) {
        Screen::drawMockup(currentGPS,currTime,sats,totalNets,openNets,clients,bat,speed,"SD Card: NOT FOUND");
        while (!SD.begin(SD_CS)) {delay(0);}
    }
    else {
         Screen::drawMockup(currentGPS,currTime,sats,totalNets,openNets,clients,bat,speed,"SD Card: FOUND");
    }

    while (true) {
        sprintf(filename,"/NuggWD-%i%.02i%.02i-%i.csv",yr, mn, dy, logNum);
        // if (!FFat.exists(filename)) { break; }
        if (!SD.exists(filename)) { break; }
        logNum++;
    }

    char tmpMessage[20];
    sprintf(tmpMessage,"Created: %s",filename);
    Screen::drawMockup("...","...",sats,totalNets,openNets,clients,bat,speed,tmpMessage);

    // file = FFat.open(String (filename), FILE_WRITE);
    file = SD.open(filename, FILE_WRITE);

    file.println(wiglePreHeader);
    file.println(wigleHeader);
    file.close();

}

void scanNets() {
    char entry[150]; 
    Serial.println("[ ] Scanning WiFi networks...");
    Screen::drawMockup(currentGPS,currTime,sats,totalNets,openNets,clients,bat,speed,"WiFi: Scanning...");

    int n = WiFi.scanNetworks();
    openNets = 0;

    #if defined(ESP8266)
        file = SD.open(filename, FILE_WRITE);
    #elif defined(ESP32)
        file = FFat.open(String (filename), FILE_APPEND);
    #endif
    
    for (int i = 0; i < n; ++i) {
        char* authType = getAuthType(WiFi.encryptionType(i));
        if (WiFi.encryptionType(i) == ENC_TYPE_NONE) openNets++;

        sprintf(entry,"%s,%s,%s,%s,%u,%i,%f,%f,%i,%f,WIFI", WiFi.BSSIDstr(i).c_str(), WiFi.SSID(i).c_str(),strDateTime,authType,WiFi.channel(i),WiFi.RSSI(i),lat,lng,alt,hdop);
        Serial.println(entry);
        file.println(entry);
    }
    totalNets+=n;

    char message[21];
    sprintf(message,"Logged %d networks.",n);
    Screen::drawMockup(currentGPS,currTime,sats,totalNets,openNets,clients,bat,speed,message);

    file.close();
    WiFi.scanDelete();
}

void getBattery() {
    float analogVal = analogRead(A0);
    // bat = map(analogVal,0,100);
    bat = 0;
}

void Wardriver::init() {
    Screen::init();
    Screen::drawSplash(2);
    
    getBattery();
    initGPS();
    initFS();
}

void Wardriver::scan() {
    updateGPS(); // poll current GPS coordinates
    getBattery();
    scanNets(); // scan WiFi nets
    smartDelay(500);
}

