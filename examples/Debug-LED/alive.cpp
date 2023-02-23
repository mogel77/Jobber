#include "Arduino.h"
#include "alive.h"



using namespace de::roboticcare::github;



ALive::ALive(uint8_t pin) : Job("ALive", 1000, 2000) { // 1x pro Sekunde wechseln, nach 2 Sekunden starten
    this->pin = pin;
    this->toggle = false;
    pinMode(pin, OUTPUT);
}
void ALive::execute(bool firstrun) {
    // firstrun ist egal
    toggle = !toggle;
    digitalWrite(pin, toggle);
}
