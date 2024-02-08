#include "Arduino.h"
#include "Filesys.h"
#include "SD.h"
#include "../Vars.h"

unsigned long startInit;
unsigned long finishInit;

/* FatFS logging or SD Card*/
#include <SD.h>

char fullFilename[120];
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

    // Filesys::configure();

}

void Filesys::configure() {

    // create config.txt if it doesn't exist
    // if (!FS_VAR.exists("config.txt")) { 
    //     File tmpSettings = FS_VAR.open("config.txt", FILE_WRITE);
        // tmpSettings.println("# Duplicates recommended");
        // tmpSettings.println("Duplicates: y");
        // tmpSettings.println("GPS RX: D4");
        // tmpSettings.println("GPS RX: D3");

        // tmpSettings.close();
    // }

    // read settings & write to variables
    // File tmpSettings = FS_VAR.open("config.txt", FILE_WRITE);
    // tmpSettings.read()

}


/* INITLIALIZE LOG FILE & WRITE HEADERS*/
void Filesys::createLog(char *filename, ScreenUpdateCallback callback) { 

    uint8_t logNum = 0;
    char wiglePreHeader[160];
    sprintf(wiglePreHeader, "WigleWifi-1.4,appRelease=%f,model=%s,release=%F,device=%s,display=SH1106,board=%s,brand=LyndLabs",VERSION,MODEL,VERSION,DEVICE,BOARD);

    // CHECK IF FILE EXISTS
    while (true) {
        sprintf(fullFilename,"/%s_%s_%i.csv",LOG_PREFIX,filename,logNum);
        if (!FS_VAR.exists(fullFilename)) { break; }
        logNum++; yield();
    }

    char tmpMessage[50];
    sprintf(tmpMessage,"Created: Log #%d",logNum);
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
    #elif defined (ESP32)
        file = FS_VAR.open(fullFilename, FILE_APPEND);
    #endif
}

void Filesys::write(char * data) {
    file.println(data);
}

void Filesys::close() {
    file.close();
}