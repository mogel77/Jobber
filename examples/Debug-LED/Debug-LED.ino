
#include "Job.h"

#include "alive.h"





void setup() {
    new ALive(LED_BUILTIN);
}

void loop() {
    de::roboticcare::github::Job::checkThreads();
    delay(1);
}
