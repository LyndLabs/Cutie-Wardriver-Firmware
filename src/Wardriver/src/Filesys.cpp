#include "Arduino.h"
#include "Filesys.h"
#include "SD.h"
#include "../Vars.h"

unsigned long startInit;
unsigned long finishInit;

/* FatFS logging or SD Card*/
#if defined(ESP8266)
    // #include <SD.h>
#elif defined(ESP32)
    #include "FFat.h"  
    #include "cdcusb.h"
    #include "mscusb.h"
    #include "flashdisk.h"
    #include "FS.h"

    FlashUSB fat1;
    CDCusb CDCUSBSerial;
    char *l1 = "ffat";

    class Device: public USBCallbacks {
        void onMount() { 
            Serial.println("Mount"); 
            finishInit = millis();  
        }
        void onUnmount() { Serial.println("Unmount"); }
        void onSuspend(bool remote_wakeup_en) { Serial.println("Suspend"); }
        void onResume() { Serial.println("Resume"); }
    };

    void echo_all(char c) {
        CDCUSBSerial.write(c);
        Serial.write(c);
    }
#endif


char fullFilename[30];
File file; 

Filesys::Filesys() {

}

void Filesys::init(Filesys::ScreenUpdateCallback callback) {

    #if defined (ESP8266)
        bool sdSuccess = SD.begin(SD_CS);
        if (!sdSuccess) {
            callback("SD Card: NOT FOUND");

            while (!sdSuccess) {
                sdSuccess = SD.begin(SD_CS);
                ESP.wdtFeed();
            }
        }
        callback("SD Card: FOUND!!");

    #elif defined (ESP32)

        EspTinyUSB::registerDeviceCallbacks(new Device());

        callback("FS: Initializing...");
        startInit = millis();
        if (fat1.init("/fat1", "ffat")) {
            if (fat1.begin()) {
                Serial.println("MSC lun 1 begin");
            }
            else {
                log_e("LUN 1 failed");
            }
        }

        if (!CDCUSBSerial.begin())
            Serial.println("Failed to start CDC USB stack");
        
        callback("FS: Starting FAT...");
        while (!FFat.begin()) { delay(0); }
        while (!fat1.available()) { delay(0); }

        delay(500);

        char tmpMsg[30];
        sprintf(tmpMsg,"FS: Done in %d ms",(finishInit-startInit));
        callback(tmpMsg);

    #endif

    Filesys::configure();

}

void Filesys::configure() {

    // create config.txt if it doesn't exist
    if (!FS_VAR.exists("config.txt")) { 
        File tmpSettings = FS_VAR.open("config.txt", FILE_WRITE);
        tmpSettings.println("# Duplicates recommended");
        tmpSettings.println("Duplicates: y");
        tmpSettings.println("GPS RX: D4");
        tmpSettings.println("GPS RX: D3");

        tmpSettings.close();
    }

    // read settings & write to variables
    // File tmpSettings = FS_VAR.open("config.txt", FILE_WRITE);
    // tmpSettings.read()

}


/* INITLIALIZE LOG FILE & WRITE HEADERS*/
void Filesys::createLog(char * filename, Filesys::ScreenUpdateCallback callback) { 

    uint8_t logNum = 0;
    char wiglePreHeader[140];
    sprintf(wiglePreHeader, "WigleWifi-1.4,appRelease=%f,model=%s,release=%F,device=%s,display=SH1106,board=%s,brand=LyndLabs",VERSION,MODEL,VERSION,DEVICE,BOARD);
    
    // CHECK IF FILE EXISTS
    while (true) {
        sprintf(fullFilename,"/%s_%s_%i.csv",LOG_PREFIX,filename,logNum);
        if (!FS_VAR.exists(fullFilename)) { break; }
        logNum++;
    }

    char tmpMessage[40];
    sprintf(tmpMessage,"LOG: Created #%d",logNum);
    callback(tmpMessage);

    // create temporary file object
    File tmpFile = FS_VAR.open(fullFilename, FILE_WRITE);

    // callback("LOG: Writing Headers");
    tmpFile.println(wiglePreHeader);
    tmpFile.println(WIGLE_HEADER);
    tmpFile.close();

    tmpFile.flush();    
    #if defined(ESP32)
        fat1.flush();
    #endif
    // callback("LOG: WROTE HEADERS");
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