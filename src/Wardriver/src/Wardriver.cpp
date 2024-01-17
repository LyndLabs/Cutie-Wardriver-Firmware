#include "Arduino.h"
#include "Wardriver.h"
#include "Recon.h"
#include "Screen.h"
#include "graphics.h"
#include "driver/temp_sensor.h"

#include <TinyGPSPlus.h>
#include "../Vars.h"
#include "Filesys.h"

#if defined(ESP8266)
#include <SoftwareSerial.h>
SoftwareSerial SERIAL_VAR(GPS_RX, GPS_TX);  // RX, TX
#endif

TinyGPSPlus gps;

// CURRENT GPS & DATTIME STRING
float lat = 0;
float lng = 0;
int alt;
double hdop;
char strDateTime[30];
char currentGPS[20] = "...";

// RECON PARAMS
uint32_t totalNets = 0;
uint8_t clients = 0;
uint8_t openNets = 0;
uint8_t sats = 0;
uint8_t bat = 0;
uint8_t speed = 0;

// DYNAMIC SCAN VARS
const int popularChannels[] = { 1, 6, 11 };
const int standardChannels[] = { 2, 3, 4, 5, 7, 8, 9, 10 };
const int rareChannels[] = { 12, 13, 14 };  // Depending on region
int timePerChannel[14] = { 300, 100, 100, 100, 100, 300, 100, 100, 100, 100, 300, 100, 100, 100 };

// DASH ICONS
char satsC[4] = "...";
char totalC[4] = "...";
char openNetsC[4] = "...";
char tmpC[4] = "...";
char batC[4] = "...";
char speedC[4] = "...";

// CURRENT DATETIME
uint8_t hr;
uint8_t mn;
uint8_t sc;
uint16_t yr;
uint8_t mt;
uint8_t dy;
char currTime[10] = "...";

char* test[6] = { satsC, totalC, openNetsC, tmpC, batC, speedC };

Wardriver::Wardriver() {
}

uint8_t getBattery() {
  // float analogVal = analogRead(A0);
  // bat = map(analogVal,0,100);
  // bat = 0;
  return 0;
}

uint8_t getTemp() {
  //   Serial.print("Temperature: ");
  float result = 0;
  temp_sensor_read_celsius(&result);

  return (int)result;
}

static void smartDelay(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (SERIAL_VAR.available())
      gps.encode(SERIAL_VAR.read());
  } while (millis() - start < ms);
}

// Utility function to check if a value is in an array
bool findInArray(int value, const int* array, int size) {
  for (int i = 0; i < size; i++) {
    if (array[i] == value) return true;
  }
  return false;
}

// TESTING: dynamic scan
void updateTimePerChannel(int channel, int networksFound) {
  const int FEW_NETWORKS_THRESHOLD = 1;
  const int MANY_NETWORKS_THRESHOLD = 5;
  const int POPULAR_TIME_INCREMENT = 75;   // Higher increment for popular channels
  const int STANDARD_TIME_INCREMENT = 50;  // Standard increment
  const int RARE_TIME_INCREMENT = 30;      // Lower increment for rare channels
  const int MAX_TIME = 500;
  const int MIN_TIME = 50;

  int timeIncrement;

  // Determine the time increment based on channel type
  if (findInArray(channel, popularChannels, sizeof(popularChannels) / sizeof(popularChannels[0]))) {
    timeIncrement = POPULAR_TIME_INCREMENT;
  } else if (findInArray(channel, rareChannels, sizeof(rareChannels) / sizeof(rareChannels[0]))) {
    timeIncrement = RARE_TIME_INCREMENT;
  } else {
    timeIncrement = STANDARD_TIME_INCREMENT;
  }

  // Adjust the time per channel based on the number of networks found
  if (networksFound >= MANY_NETWORKS_THRESHOLD) {
    timePerChannel[channel - 1] += timeIncrement;
    if (timePerChannel[channel - 1] > MAX_TIME) {
      timePerChannel[channel - 1] = MAX_TIME;
    }
  } else if (networksFound <= FEW_NETWORKS_THRESHOLD) {
    timePerChannel[channel - 1] -= timeIncrement;
    if (timePerChannel[channel - 1] < MIN_TIME) {
      timePerChannel[channel - 1] = MIN_TIME;
    }
  }
}

void updateGPS() {
  lat = gps.location.lat();
  lng = gps.location.lng();
  alt = (int)gps.altitude.meters();
  hdop = gps.hdop.hdop();
  sats = gps.satellites.value();
  speed = gps.speed.mph();

  yr = gps.date.year();
  mt = gps.date.month();
  dy = gps.date.day();

  hr = gps.time.hour();
  mn = gps.time.minute();
  sc = gps.time.second();

  sprintf(strDateTime, "%04d-%02d-%02d %02d:%02d:%02d", yr, mt, dy, hr, mn, sc);
  sprintf(currentGPS, "%1.3f,%1.3f", lat, lng);
  sprintf(currTime, "%02d:%02d", hr, mn);
  sprintf(satsC, "%u", sats);

  if (totalNets > 999) {
    // sprintf(totalC,"%uK",((totalNets-1)/1000));
    sprintf(totalC, "%gK", ((totalNets) / 100) / 10.0);
    // Serial.println(((totalNets-1)/100)/10.0);
  } else {
    sprintf(totalC, "%u", totalNets);
  }

  sprintf(openNetsC, "%u", openNets);
  uint8_t tmpTemp = getTemp();
  sprintf(tmpC, "%u", tmpTemp);
  sprintf(batC, "%u", getBattery());
  sprintf(speedC, "%u", speed);

  Screen::setFooter("GPS: UPDATED");
  Screen::update();
}

void updateGPS(uint8_t override) {
  lat = 37.8715;
  lng = 122.2730;
  alt = 220;
  hdop = 1.5;
  sats = 3;
  speed = 69;

  yr = 2023;
  mt = 7;
  dy = 25;
  hr = 10;
  mn = 36;
  sc = 56;

  sprintf(strDateTime, "%i-%i-%i %i:%i:%i", yr, mt, dy, hr, mn, sc);
  sprintf(currentGPS, "%1.3f,%1.3f", lat, lng);
  sprintf(currTime, "%02d:%02d", hr, mn);

  sprintf(satsC, "%u", sats);

  if (totalNets - 1 > 999) {
    // sprintf(totalC,"%uK",((totalNets-1)/1000));
    sprintf(totalC, "%gK", ((totalNets - 1) / 100) / 10.0);
    // Serial.println(((totalNets-1)/100)/10.0);
  } else {
    sprintf(totalC, "%u", totalNets - 1);
  }

  sprintf(openNetsC, "%u", openNets);
  sprintf(tmpC, "%u°", getTemp());
  sprintf(batC, "%u%", getBattery());
  sprintf(speedC, "%u", speed);

  Screen::setFooter("GPS: UPDATED");
  Screen::update();
  // Screen::drawMockup("...","...",0,0,0,0,0,0,"test");
}

// initialize GPS & get first coords
void initGPS() {

#if defined(ESP32)
  SERIAL_VAR.begin(GPS_BAUD, SERIAL_8N1, GPS_RX, GPS_TX);
#elif defined(ESP8266)
  SERIAL_VAR.begin(GPS_BAUD);
#endif

  // char *test[6] = {"1","2","3","23","5","6"};
  // Screen::setIcons(icons_bits, test, 6);
  // Screen::setHeader(currentGPS,currTime);   // pass ref
  Screen::setFooter("GPS Initializing...");  //
  Screen::update();

  unsigned long startGPSTime = millis();

  while (!(gps.location.isValid())) {

    if (millis() - startGPSTime > 5000 && gps.charsProcessed() < 10) {
      Screen::setFooter("GPS: NOT FOUND");
      Screen::update();

    } else if (gps.charsProcessed() > 10) {
      Screen::setFooter("GPS: Waiting for fix...");
      Screen::update();
    }

    sats = gps.satellites.value();
    // totalNets = 0;
    Serial.println(gps.location.isValid());
    //ESP.wdtFeed();
    yield();

    smartDelay(500);
  }
  while ((gps.date.year() == 2000)) {
    Screen::setFooter("GPS: Validating time...");
    //ESP.wdtFeed();
    yield();
    smartDelay(500);
    Serial.println(gps.date.year());
  }
  Screen::setFooter("GPS: LOCATION FOUND");
  updateGPS();
}

void initGPS(uint8_t override) {
#if defined(ESP32)
  // SERIAL_VAR.begin(GPS_BAUD, SERIAL_8N1, GPS_RX, GPS_TX);
#endif
  Screen::setFooter("GPS: Emulating fix");
  Screen::update();
  delay(500);

  updateGPS(0);
  Screen::setFooter("GPS: LOCATION FOUND");
  Screen::update();
  Serial.println("Screen updated.");
}

void scanNets() {
    char entry[250];
    Serial.println("[ ] Scanning WiFi networks...");
    Screen::setFooter("WiFi: Scanning...");
    Screen::update();

    Filesys::open();
    int numNets = 0; // Reset at each scan call

    auto processNetworks = [&](int networksFound) {
        for (int i = 0; i < networksFound; i++) {
            char* authType = getAuthType(WiFi.encryptionType(i));
#if defined(ESP8266)
            if (WiFi.encryptionType(i) == ENC_TYPE_NONE) openNets++;
#elif defined(ESP32)
            if (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) openNets++;
#endif

            sprintf(entry, "%s,\"%s\",%s,%s,%u,%i,%f,%f,%i,%f,WIFI", WiFi.BSSIDstr(i).c_str(), WiFi.SSID(i).c_str(), authType, strDateTime, WiFi.channel(i), WiFi.RSSI(i), lat, lng, alt, hdop);

            Serial.println(entry);
            Filesys::write(entry);
            totalNets++;
            numNets++;
        }
    };

    if (Filesys::dynamicScan) {
        for (int channel = 1; channel <= 14; channel++) {
            int networksOnChannel = WiFi.scanNetworks(false, true, false, timePerChannel[channel - 1], channel);
            processNetworks(networksOnChannel);
            updateTimePerChannel(channel, networksOnChannel);
        }
    } else {
        int networksFound = Filesys::showHidden ? WiFi.scanNetworks(false, true, false, Filesys::timePerChan) : WiFi.scanNetworks();
        processNetworks(networksFound);
    }

    char message[30];
    sprintf(message, "Logged %d Networks", numNets);
    Screen::setFooter(message);
    Screen::update();

    Filesys::close();
    WiFi.scanDelete();
}


void Wardriver::init() {

  totalNets = 0;
  temp_sensor_config_t temp_sensor = TSENS_CONFIG_DEFAULT();
  temp_sensor.dac_offset = TSENS_DAC_L2;  // TSENS_DAC_L2 is default; L4(-40°C ~ 20°C), L2(-10°C ~ 80°C), L1(20°C ~ 100°C), L0(50°C ~ 125°C)
  temp_sensor_set_config(temp_sensor);
  temp_sensor_start();

  Screen::init();
  Screen::drawSplash(2);
  Screen::setIcons(icons_bits, test, 6);
  Screen::setHeader(currentGPS, currTime);

  Filesys::init(updateScreen);
  delay(1000);

  // getBattery();

  initGPS();
  char fileDT[150];
  sprintf(fileDT, "%i-%02d-%02d", yr, mt, dy);
  delay(1000);
  Filesys::createLog(fileDT, updateScreen);
}

void Wardriver::updateScreen(char* message) {
  Screen::setFooter(message);
  Screen::update();
}

void Wardriver::scan() {
  Serial.println("In scan.");
  updateGPS();  // poll current GPS coordinates
  // getBattery();
  scanNets();  // scan WiFi nets
  smartDelay(500);
}
