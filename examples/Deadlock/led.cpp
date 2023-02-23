#include "Arduino.h"
#include "led.h"



using namespace de::roboticcare::github;



LED::LED(uint8_t pin, uint16_t timeout) : Job("ALive", timeout, 2000) {
    this->pin = pin;
    this->toggle = false;
    pinMode(pin, OUTPUT);
}
void LED::execute(bool firstrun) {
    // firstrun ist egal
    toggle = !toggle;
    digitalWrite(pin, toggle);
}
