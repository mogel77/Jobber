#include "Arduino.h"
#include "dead.h"



using namespace de::roboticcare::github;



Deadlock::Deadlock(uint8_t pin) : Job("ALive", 100, 2000) { // schnelles Abfragen
    this->pin = pin;
    pinMode(pin, INPUT_PULLUP);
}
void Deadlock::execute(bool firstrun) {
    // firstrun ist egal
    while(!digitalRead(pin));
}
