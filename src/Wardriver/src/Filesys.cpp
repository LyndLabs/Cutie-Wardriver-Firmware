#include "Arduino.h"
#include "Filesys.h"
#include <SD.h>
#include "../Vars.h"

/* FatFS logging or SD Card*/
#if defined(ESP8266)
    
#elif defined(ESP32)
    #include "FFat.h"  
    #include "cdcusb.h"
    #include "mscusb.h"
    #include "flashdisk.h"
    #include "FS.h"

    FlashUSB fat1;
    CDCusb CDCUSBSerial;

    char *l1 = "ffat";

    class MyCDCCallbacks : public CDCCallbacks {

};

class Device: public USBCallbacks {
    void onMount() { Serial.println("Mount"); }
    void onUnmount() { Serial.println("Unmount"); }
    void onSuspend(bool remote_wakeup_en) { Serial.println("Suspend"); }
    void onResume() { Serial.println("Resume"); }
};

void echo_all(char c) {
    CDCUSBSerial.write(c);
    Serial.write(c);
}
#endif



char filename[23];
File file; 

Filesys::Filesys() {

}

// eventually use callbacks
void Filesys::init(char * filename, Filesys::ScreenUpdateCallback callback) {
    uint8_t logNum = 0;
    char * wiglePreHeader = "WigleWifi-1.4,appRelease=1.0,model=DevKit,release=1.0,device=DNS Driveby,display=SH1106,board= ESP8266,brand=LyndLabs";
    char * wigleHeader    = "MAC,SSID,AuthMode,FirstSeen,Channel,RSSI,CurrentLatitude,CurrentLongitude,AltitudeMeters,AccuracyMeters,Type";
    
    // Assign file name
    // #if defined (ESP8266) 
    //     char * deviceName = "DNS Driveby"
    // #elif defined(ESP32)
    //     char * deviceName = "WiFi Nugget"
    // #endif

    if (!SD.begin(SD_CS)) {
        callback("SD Card: NOT FOUND");
        ESP.wdtDisable();
        while (!SD.begin(SD_CS)) { delay(0); }
    }
    else {
        callback("SD Card: FOUND!!");
    }

    while (true) {
        char fullFilename[30];
        sprintf(fullFilename,"/NuggWD-%s-%i.csv",filename, logNum);
        // if (!FFat.exists(filename)) { break; }
        if (!SD.exists(fullFilename)) { break; }
        logNum++;
    }

    char tmpMessage[20];
    sprintf(tmpMessage,"Created: %s",filename);
    callback(tmpMessage);

    // file = FFat.open(String (filename), FILE_WRITE);
    file = SD.open(filename, FILE_WRITE);
    file.println(wiglePreHeader);
    file.println(wigleHeader);
    file.close();
}

void Filesys::open() {
    #if defined(ESP8266)
        file = SD.open(filename, FILE_WRITE);
    #elif defined(ESP32)
        file = FFat.open(String (filename), FILE_APPEND);
    #endif
}

void Filesys::write(char * data) {
    file.println(data);
}

void Filesys::close() {
    file.close();
}