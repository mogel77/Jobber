
#include "Job.h"

#include "led.h"
#include "dead.h"





void setup() {
    new LED(LED_BUILTIN, 500);

    // Pin 12 -> Resistor 1k -> GND = Deadlock
    new Deadlock(12);
}

void loop() {
    de::roboticcare::github::Job::checkThreads();
    delay(1);
}
