/*

    simulated Multithreading for Arduino & Co.

    Copyright (C) 2023 robotic care GmbH / r.gorzelitz

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
    USA

*/



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
