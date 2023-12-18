#include "Arduino.h"
#include "Wardriver.h"
#include "Recon.h"
#include "Screen.h"

#include <TinyGPSPlus.h>
#include "../Vars.h"
#include "Filesys.h"

#if defined(ESP8266)
    #include <SoftwareSerial.h>
    SoftwareSerial SERIAL_VAR(GPS_RX, GPS_TX); // RX, TX
#endif

TinyGPSPlus gps;

// CURRENT GPS & DATTIME STRING
float lat = 0; float lng = 0;
int alt; double hdop;
char strDateTime[30];
char currentGPS[20];

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
char currTime[10];

Wardriver::Wardriver() {

}

static void smartDelay(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (SERIAL_VAR.available())
      gps.encode(SERIAL_VAR.read());
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

    sprintf(strDateTime,"%04d-%02d-%02d %02d:%02d:%02d",yr,mt,dy,hr,mn,sc);
    sprintf(currentGPS,"%1.3f,%1.3f",lat,lng);
    sprintf(currTime,"%02d:%02d",hr,mn);

    Screen::drawMockup(currentGPS,currTime,sats,totalNets,openNets,clients,bat,speed,"GPS: UPDATED");
}

void updateGPS(uint8_t override) {
    lat = 37.8715; lng = 122.2730;
    alt = 220; hdop = 1.5;
    sats = 3; speed = 69;

    yr = 2023; mt = 7; dy = 25;
    hr = 10; mn = 36; sc = 56;

    sprintf(strDateTime,"%i-%i-%i %i:%i:%i",yr,mt,dy,hr,mn,sc);
    sprintf(currentGPS,"%1.3f,%1.3f",lat,lng);
    sprintf(currTime,"%02d:%02d",hr,mn);

    Screen::drawMockup(currentGPS,currTime,sats,totalNets,openNets,clients,bat,speed,"GPS: UPDATED");
}

// initialize GPS & get first coords
void initGPS() {

    #if defined(ESP32)
        SERIAL_VAR.begin(GPS_BAUD, SERIAL_8N1, GPS_RX, GPS_TX);
    #elif defined(ESP8266)
        SERIAL_VAR.begin(GPS_BAUD);
    #endif

    Screen::drawMockup("...","...",sats,totalNets,openNets,clients,bat,speed,"GPS: Initializing...");

    unsigned long startGPSTime = millis();
    while (! (gps.location.isValid())) {
        if (millis()-startGPSTime > 5000 && gps.charsProcessed() < 10) {
            Screen::drawMockup("...","...",sats,totalNets,openNets,clients,bat,speed,"GPS: NOT FOUND");
        }
        else if (gps.charsProcessed() > 10) {
            Screen::drawMockup("...","...",sats,totalNets,openNets,clients,bat,speed,"GPS: Waiting on fix...");
        }
        sats = gps.satellites.value();
        
        Serial.println(gps.location.isValid());
        //ESP.wdtFeed(); 
        yield();

	smartDelay(500);
    }
    while ((gps.date.year() == 2000)) {
        Screen::drawMockup("...","...",sats,totalNets,openNets,clients,bat,speed,"GPS: Validating time...");
        //ESP.wdtFeed(); 
        yield();
	smartDelay(500);
        Serial.println(gps.date.year());
    }
    Screen::drawMockup("...","...",sats,totalNets,openNets,clients,bat,speed,"GPS: LOCATION FOUND");

    updateGPS();
}

void initGPS(uint8_t override) {
    #if defined(ESP32)
        // SERIAL_VAR.begin(GPS_BAUD, SERIAL_8N1, GPS_RX, GPS_TX);
    #endif
    Screen::drawMockup("...","...",0,0,0,0,0,0,"GPS: Emulating fix");
    delay(500);

    updateGPS(0);
    Screen::drawMockup(currentGPS,currTime,sats,totalNets,openNets,clients,bat,speed,"GPS: LOCATION FOUND");
}

void scanNets() {
    char entry[150]; 
    Serial.println("[ ] Scanning WiFi networks...");
    Screen::drawMockup(currentGPS,currTime,sats,totalNets,openNets,clients,bat,speed,"WiFi: Scanning...");

    int n = WiFi.scanNetworks();
    openNets = 0;

    Filesys::open();
    
    for (int i = 0; i < n; ++i) {
        char* authType = getAuthType(WiFi.encryptionType(i));
        #if defined(ESP8266)
            if (WiFi.encryptionType(i) == ENC_TYPE_NONE) openNets++;
        #elif defined(ESP32)
            if (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) openNets++;
        #endif

        sprintf(entry,"%s,\"%s\",%s,%s,%u,%i,%f,%f,%i,%f,WIFI", WiFi.BSSIDstr(i).c_str(), WiFi.SSID(i).c_str(),authType,strDateTime,WiFi.channel(i),WiFi.RSSI(i),lat,lng,alt,hdop);
								
        Serial.println(entry);
        Filesys::write(entry);
    }
    totalNets+=n;

    char message[21];
    sprintf(message,"Logged %d networks.",n);
    Screen::drawMockup(currentGPS,currTime,sats,totalNets,openNets,clients,bat,speed,message);

    Filesys::close();
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

    Filesys::init(updateScreen); delay(1000);
    // getBattery();
    initGPS();
    char fileDT[40]; sprintf(fileDT,"%i-%02d-%02d",yr, mt, dy);
    Serial.println(fileDT);
    delay(1000);
    Filesys::createLog(fileDT, updateScreen);
}

void Wardriver::updateScreen(char* message) {
    Screen::drawMockup(currentGPS,currTime,sats,totalNets,openNets,clients,bat,speed,message);
}

void Wardriver::scan() {
    updateGPS(); // poll current GPS coordinates
    getBattery();
    scanNets(); // scan WiFi nets
    smartDelay(500);
}

