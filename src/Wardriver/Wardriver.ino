#include "Vars.h"
#include "src/Wardriver.h"

void setup() {
    Serial.begin(115200);
    Wardriver::init(); // start screen + gps
}

void loop() {
    Wardriver::scan();
}