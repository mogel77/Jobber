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



#include <Arduino.h>




#include "Job.h"



using namespace de::roboticcare::repraxi;
using namespace std;


// List<Job>* Job::threadList = new List<Job>();
vector<Job*> Job::threadList;
uint32_t Job::jobcounter = 1;



// statischer Scheiß der Klasse
void Job::checkThreads() {
    // SERIAL_DEBUG.printf("    %i Jobs\n", threadList.size());
    for(vector<Job*>::iterator iterator = threadList.begin(); iterator != threadList.end(); ++iterator) {
        Job* job = (*iterator);
        //SERIAL_DEBUG.printf("Job: %s\n", job->getJobName())
        job->check();
    }
}
Job* Job::findJob(uint32_t id) {
    for(vector<Job*>::iterator iterator = threadList.begin(); iterator != threadList.end(); ++iterator) {
        Job* job = *iterator;
        if (job->getJobID() == id) return job;
    }
    return nullptr;
}



// per Instanz



Job::Job(const char* name, uint32_t timeout) {
    init(name, timeout, 0);
}
Job::Job(const char* name, uint32_t timeout, uint32_t delay) {
    init(name, timeout, delay);
}
void Job::init(const char* name, uint32_t timeout, uint32_t delay) {
    firstrun = true;
    enabled = true;
	this->lastAction = millis() + delay;
	this->timeout = timeout;
    // SERIAL_DEBUG.printf("%8ums THREAD-Init: %s - Timeout: %ims - Next: %8u\n", millis(), name, this->timeout, this->lastAction);
    for(int i = 0; i < THREAD_CYCLE_SIZE; i++) cycleDuration[i] = 0;
    snprintf(this->jobname, THREAD_NAME_SIZE - 1, "%s", name);
    overloadCount = 0;
    cycleCurrentLoad = 0;
    jobId = jobcounter++;
    threadList.insert(threadList.end(), this);
    snprintf(this->errormessage, THREAD_ERROR_SIZE - 1, "%s", "running perfekt");
}
void Job::reset() {
    this->lastAction = millis();
}
void Job::disable() {
    enabled = false;
}
void Job::enable() {
	enabled = true;
}
void Job::check() {
    if (millis() < lastAction) return;
    if (enabled && (millis() - lastAction >= timeout) ) {
        lastAction = millis();
        uint32_t start = micros();
        //SERIAL_DEBUG.printf(">>> execute (%s:%i) - begin\n", jobname, jobId);
        execute(firstrun);
        //SERIAL_DEBUG.printf("<<< execute (%s:%i) - end\n", jobname, jobId);
        firstrun = false;
        int32_t measured = micros() - start;
        // wir hatten einen Überlauf - diese Runde nix berechnen
        if (measured < 0) { lastAction = 0; return; }

        // Durchschnittliche Auslastung berechnen
        cycleCurrentLoad = 0;
        for(int i = 1; i < THREAD_CYCLE_SIZE; i++) cycleDuration[i - 1] = cycleDuration[i];     // um eins nach vorne verschieben
        cycleDuration[THREAD_CYCLE_SIZE - 1] = measured;                                        // aktueller Stand
        for(int i = 0; i < THREAD_CYCLE_SIZE; i++) cycleCurrentLoad += cycleDuration[i];        // summieren
        cycleCurrentLoad /= THREAD_CYCLE_SIZE;

        // in Prozent wandeln
        cycleCurrentLoad = (((cycleCurrentLoad / (timeout * 1.f)) * 100.0f) / 1000);      // timout sind ms - load zählt in us - daher umrechnen

        if (cycleCurrentLoad > 100) {
            overloadCount++;
            if (overloadCount == 5) SERIAL_DEBUG.printf("Thread: '%s' ist überlastet mit %i%%\n", jobname, cycleCurrentLoad);
        } else {
            if (overloadCount >= 5) SERIAL_DEBUG.printf("Thread: Überlastung für '%s' zurückgesetzt\n", jobname);
            overloadCount = 0;
        }
    }
}
