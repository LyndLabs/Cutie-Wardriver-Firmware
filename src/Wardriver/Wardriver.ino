#include "Vars.h"
#include "src/Wardriver.h"

void setup() {
    Serial.begin(115200);
    pinMode(A0, INPUT);
    Wardriver::init(); 
}

void loop() {
    Wardriver::scan();
}