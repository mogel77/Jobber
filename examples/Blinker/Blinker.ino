
#include "Job.h"

#include "led.h"





void setup() {
    new LED(LED_BUILTIN, 500);
    new LED(12, 1000);
}

void loop() {
    de::roboticcare::github::Job::checkThreads();
    delay(1);
}
