#include "Arduino.h"
#include "Filesys.h"
#include "SD.h"
#include "../Vars.h"

unsigned long startInit;
unsigned long finishInit;

// Define these is config.txt on SD as value=true/false, one per line
bool Filesys::dedupe = false;
bool Filesys::showHidden = false;
int Filesys::timePerChan = 250;

/* FatFS logging or SD Card*/
#include <SD.h>

char fullFilename[100];
File file;

Filesys::Filesys() {
}

void Filesys::init(ScreenUpdateCallback callback) {
  bool sdSuccess = SD.begin(SD_CS);
  if (!sdSuccess) {
    callback("SD Card: NOT FOUND");

    while (!sdSuccess) {
      sdSuccess = SD.begin(SD_CS);
      //ESP.wdtFeed();
      yield();
    }
  }
  callback("SD Card: FOUND!!");

  Filesys::configure();
}

void Filesys::configure() {
  File configFile = SD.open("/config.txt");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return;
  }

  while (configFile.available()) {
    String line = configFile.readStringUntil('\n');
    int separatorPos = line.indexOf('=');
    if (separatorPos != -1) {
      String key = line.substring(0, separatorPos);
      String value = line.substring(separatorPos + 1);
      // format: param=true/false, one per line
      if (key == "dedupe") {
        dedupe = (value == "true");
        Serial.print("dedupe ");
        Serial.println(dedupe ? "true" : "false");
      } else if (key == "showHidden") {
        showHidden = (value == "true");
        Serial.print("showHidden ");
        Serial.println(showHidden ? "true" : "false");
      } else if (key == "timePerChan") {
        timePerChan = value.toInt();
        Serial.print("timePerChan ");
        Serial.println(timePerChan);
      }
      // others config vars
    }
  }
  configFile.close();
}


/* INITLIALIZE LOG FILE & WRITE HEADERS*/
void Filesys::createLog(char *filename, ScreenUpdateCallback callback) {

  uint8_t logNum = 0;
  char wiglePreHeader[140];
  sprintf(wiglePreHeader, "WigleWifi-1.4,appRelease=%f,model=%s,release=%F,device=%s,display=SH1106,board=%s,brand=LyndLabs", VERSION, MODEL, VERSION, DEVICE, BOARD);

  // CHECK IF FILE EXISTS
  while (true) {
    sprintf(fullFilename, "/%s_%s_%i.csv", LOG_PREFIX, filename, logNum);
    if (!FS_VAR.exists(fullFilename)) { break; }
    logNum++;
    yield();
  }

  char tmpMessage[50];
  sprintf(tmpMessage, "Created: Log #%d", logNum);
  callback(fullFilename);

  // create temporary file object
  File tmpFile = FS_VAR.open(fullFilename, FILE_WRITE);

  // callback("LOG: Writing Headers");
  tmpFile.println(wiglePreHeader);
  tmpFile.println(WIGLE_HEADER);
  tmpFile.close();

  tmpFile.flush();
#if defined(ESP32)
  //fat1.flush();
#endif
}

void Filesys::open() {
#if defined(ESP8266)
  file = FS_VAR.open(fullFilename, FILE_WRITE);
#elif defined(ESP32)
  file = FS_VAR.open(fullFilename, FILE_APPEND);
#endif
}

void Filesys::write(char *data) {
  file.println(data);
}

void Filesys::close() {
  file.close();
}