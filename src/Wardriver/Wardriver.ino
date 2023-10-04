#include "Vars.h"
#include "src/Wardriver.h"

void setup() {
    Serial.begin(115200);
    pinMode(A0, INPUT);
    ESP.wdtDisable();
    
    Wardriver::init(); 
    ESP.wdtEnable(0);
}

void loop() {
    Wardriver::scan();
}