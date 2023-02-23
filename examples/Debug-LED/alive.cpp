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
