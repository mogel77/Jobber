**other Librarys: ArduinoSTL**

# Jobber [EN]
Simplified thread management for Arduino and others.

# Challenges

The "ArduinoSTL" package produces errors but is fixable. To do this, simply do the post by "rw3iss" (https://github.com/mike-matera/ArduinoSTL/issues/84).

If to use the `std::vector` directly, set the #define 'NO_ARDUINO_STL' to the build (-D NO_ARDUINO_STL).

## Contents

### Simplified Threading

For each job, a call is made every x milliseconds. However, this also means that no job may run longer than this period of time. Example:

  - Job A should be called every 50ms
  - Job B should be called every 100ms
  - Job B needs **75ms** to process
  - this means that job A can only be processed every 75 ms

### Race Conditions

In most cases, race conditions are avoided because the jobs are always processed one after the other. Interrupts are an exception. These are always used by the Arduino framework. For example, outputs via `Serial` are controlled by the framework via interrupts. A `Serial.flush()` helps here to wait for the end of the transfer.

If interrupts are to be used, e.g. timing and data must be exchanged there. Then you have to pay attention to synchronization yourself.

### Deadlock

If a job comes into a deadlock, then all other jobs are also blocked.

## Application

Only the call `Job::checkThreads()` has to be in the `loop()`. Here it is checked which job has to be executed once via `Job::execute()`. The `delay(1)` is not a must.

```
void loop() {
    de::roboticcare::github::Job::checkThreads();
    delay(1);
}
```

### Example: Debug LED
The example shows the basic use of the job class. To do this, the `LED_BUILTIN` is switched on and off once per second.

### Example: Blink

2 LEDs flash. Once every 1000ms and once every 500ms. This example shows the simple use of asynchronous and repetitive tasks.

### Example: Deadlock

If PIN12 is connected to GND with a resistor of 1kOhm, then the job blocks 'Deadlock'. The LED only flashes again when the connection from Pin12 to GND is interrupted.

# Jobber [DE]
Vereinfachtes Thread-Management für Arduino und Andere.

## Inhalt

### vereinfachtes Threading

Für jeden Job wird ein Aufruf alle x Millisekunden ausgeführt. Dies bedeutet aber auch, das kein Job länger als diese Zeitspanne laufen darf. Bespiel:

 - Job A soll alls 50ms aufgerufen werden
 - Job B soll alle 100ms aufgerufen werden
 - Job B benötigt aber **75ms** zum abarbeiten
 - dadurch kann Job A nur alle 75ms abgearbeitet werden

### Race-Conditions

In den meisten Fällen werden Race-Conditions verhindert, da die Jobs immer hintereinander abgearbeitet werden. Eine Ausnahme bilden Interrupts. Diese werden immer vom Arduino-Framework verwendet. Zum Bespiel werden Ausgaben über `Serial` über Interrups vom Framework gesteuert. Hier hilft ein `Serial.flush()` um auf das Ende der Übertragung zu warten.

Wenn Interrupts verwendet werden sollen, z.B. Timing und dort Daten gestauscht werden müssen. Dann ist selber auf eine Synchronisation zu achten.

### Deadlocks

Wenn ein Job in eine Deadlock kommt, dann werden auch alle anderen Jobs blockiert.

## Anwendung

Im `loop()` muss nur der Aufruf `Job::checkThreads()` stehen. Hier wird geprüft welcher Job über `Job::execute()` einmal ausgeführt werden muss. Das `delay(1)` ist kein muss.

```
void loop() {
   de::roboticcare::github::Job::checkThreads();
   delay(1);
}
```

### Beispiel: Debug-LED
Das Beispiel zeigt die prinzipelle Verwendung der Job-Klasse. Dazu wird die `LED_BUILTIN` einmal pro Sekunde an bzw. aus geschaltet.

### Beispiel: Blink

Es blinken 2 LED. Einmal alle 1000ms und einmal alle 500ms. Dieses beispiel zeigt die einfache Verwendung von asynchronen und wiederholenden Aufgaben.

### Beispiel: Deadlock

Wird der PIN12 mit einem Widerstand von 1kOhm zu GND verbunden, dann blockiert der Job 'Deadlock'. Die LED blinkt erst wieder, wenn die Verbindung von Pin12 zu GND unterbrochen wird.
