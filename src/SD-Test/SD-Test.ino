#include "FFat.h"  
#include "cdcusb.h"
#include "mscusb.h"
#include "flashdisk.h"
#include "FS.h"
#include 

FlashUSB fat1;
CDCusb CDCUSBSerial;
File file;

char *l1 = "ffat";

char fullFilename[30];

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
/* setup and loop functions here */

void writeStuff() {
    uint16_t randNumber = random(10, 40);
    Serial.printf("Created rand number %d",randNumber);
    file = FFat.open(String (fullFilename), FILE_APPEND);

    for (uint16_t i=0; i<randNumber; i++) {
        file.println(i);
        Serial.println(i);
    }
    file.close();
}

void setup() {
    Serial.begin(115200);

     if (fat1.init("/fat1", "ffat")) {
        if (fat1.begin()) {
            Serial.println("MSC lun 1 begin");
        }
        else
            log_e("LUN 1 failed");
        }
        if (!CDCUSBSerial.begin())
            Serial.println("Failed to start CDC USB stack");

        // CDCUSBSerial.setCallbacks(new MyCDCCallbacks());
        EspTinyUSB::registerDeviceCallbacks(new Device());
        delay(1000);

        if(!FFat.begin(true)){ Serial.println("USB: FatFS ERROR"); }
        else {
            Serial.println("Started FATFS!");
            delay(500);
        }
        createFile();
        writeStuff();
        Serial.println("done!");
}

void loop() {
    // nothin to do here
}

void createFile() {
    uint16_t logNum=0;
    char * filename = "tester";
    Serial.println("Trying to create file...");
     while (true) {
        sprintf(fullFilename,"/NuggWD-%s-%i.csv",filename, logNum);
        if (!FFat.exists(fullFilename)) { break; }
        logNum++;
    }
    Serial.printf(" %s created!", fullFilename);
    Serial.println();
}

