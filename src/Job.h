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



#ifndef DE_ROBOTICCARE_GITHUB_JOBBER_H
#define DE_ROBOTICCARE_GITHUB_JOBBER_H





#include <Arduino.h>
#include <ArduinoSTL.h>



/** Anzahl der Zyklen, die gemessen wird */
#ifndef JOB_CYCLE_SIZE
    #define JOB_CYCLE_SIZE           (8)
#endif

/** Platz für den Thread-Namen */
#ifndef JOB_NAME_SIZE
    #define JOB_NAME_SIZE            (20)
#endif

/** Platz für Fehlermeldungen der Threads - nur intern */
#ifndef JOB_ERROR_SIZE
    #define JOB_ERROR_SIZE           (100)
#endif

/** Debug-Ausgabe */
#ifndef SERIAL_DEBUG
    #define SERIAL_DEBUG            (Serial)
#endif





namespace de { namespace roboticcare { namespace github {



/**
 * @brief eine Art Thread für den µC
 * 
 * Die Überprüfung ob ein Jab argearbeitet wird, erfolgt alle 1ms.
 * Jeder Job bekommt eine Aufrufzeit, z.B. alls 100ms. Wenn die Zeit
 * abgelaufen ist, wird der Job ausgeführt @see execute.
 * 
 * Im Grunde garantiert die Job-Klasse das alle Jobs synchron ausgeführt
 * wird, also immer nur eine Hintereinander. Problematisch ist dadurch
 * aber, das der Jobs mit der geringsten Zeit die Geschwindigkeit der
 * Abarbeitung festlegt. Benötigt ein Job immer mind. 56ms, dann kann kein
 * Job schneller als alle 56ms aufgerufen werden.
 */
class Job {

    //
    //  -- Static --
    //
    public:
        /** alle bekannten Jobs */
        static std::vector<Job*>& getThreadList() { return threadList; }
        /** alle Jobs einmal abarbeiten */
        static void checkThreads();
        /** Job über ID finden */
        static Job* findJob(uint32_t id);

    protected:
        static std::vector<Job*> threadList;

    private:
        static uint32_t jobcounter;

    //
    //  -- per Instance --
    //
    public:
        /**
         * @brief Construct a new Job object
         * 
         * @param name Name des Jobs (max. 19 Zeichen)
         * @param timeout Aufrufgeschwindigkeit in ms
         */
        Job(const char* name, uint32_t timeout);
        /**
         * @brief Construct a new Job object
         * 
         * @param name Name des Jobs (max. 19 Zeichen)
         * @param timeout Aufrufgeschwindigkeit in ms
         * @param delay zusätzliche Pause vor dem ersten Aufruf
         */
        Job(const char* name, uint32_t timeout, uint32_t delay);
        
        /** deaktiviert den Job */
        void disable();
        /** aktiviert den Job */
        void enable();
        /** sort dafür, das der Job beim nächsten Check @see checkThreads ausgeführt wird */
        void reset();

        /** Ändert den Timeout zur Abarbeitung
         * @remark eine Abarbeitung erfolgt erst nach dem Nächsten regulärem Check @see checkThreads */
        void setTimeout(uint32_t timeout) { this->timeout = timeout; this->lastAction = millis() + timeout; }

        /** die Auslastung der CPU für diesen Job in Prozent */
        uint32_t getCPULoad()           { return cycleCurrentLoad; }
        /** Dauer der letzten Abarbeitung in ms */
        uint32_t getCPUTime()           { return (cycleDuration[0] / 1000); }    // in ms - sollte grob passen
        /** der Name dises Jobs */
        char* getJobName()              { return jobname; }
        /** gibt die ungefähre Zeit an, wie lange der Job schon überladen ist 
         * @remark ein Job ist überladen, wenn die Abarbeitungszeit länger ist, als der Aufruf durch @see checkThreads */
        uint32_t getOverloadTime()      { return (overloadCount * timeout); }
        /** TRUE wenn der Job aktiviert ist */
        bool isEnabled()                { return enabled; }
        /** Abstand zwischen zeit Abarbeitungen @see checkThreads */
        uint32_t getTimeout()           { return timeout; }
        /** die ID des Jobs - kann sich bei jedem Start ändern */
        uint32_t getJobID()             { return jobId; }
        /** die mögliche Fehlermeldung, wenn ein Fehler auftritt bzw. aufgetreten ist */
        const char* getErrorMessage()   { return errormessage; }

    protected:
        /** @brief legt die Fehlermeldung fest
         * @param die eigentliche Fehlermeldung, max. @see THREAD_ERROR_SIZE Bytes */
        void setErrorMessage(const char* message) { snprintf(errormessage, JOB_ERROR_SIZE - 1, message); }
        /** löscht den Fehler und setzt die Default-Mmeldung 
         * @remark zu jedem Anfang der Abarbeitung ausgeführt. Ist ein Job deaktiviert @see disable, wird
         * die Methode nicht aufgerufen und die letzte erzeugte Fehlermeldung kann über die Konsole abgerufen werden.
        */
        void flushErrorMessage() { setErrorMessage("running perfekt"); }
        /** interne Aufruf um den Job abzuarbeiten @see execute */
        void check();

    private: 
        /**
         * @brief führt die Abarbeitung eines Job durch
         * @remark es darf keine Endlosschleife enthalten sein
         * @param firstrun TRUE wenn es der erste Durchlauf ist, ermöglicht 
         * spätere Initialisierungen die nicht im .ctor() gemacht werden können
         */
        virtual void execute(bool firstrun) = 0;
        /**
         * @brief initialisiert den Job, wird von .ctor() aufgerufen
         * @param jobname der über die Konsole anzuzeigende Namen
         * @param timeout Zeit zwischen zwei Abarbeitungspunkten (in ms)
         * @param delay anfängliche Verzögerung vor dem ersten Durchlauf (in ms)
         */
        void init(const char* jobname, uint32_t timeout, uint32_t delay);            // Initialisierung in eine Methode

    private:
        bool firstrun;
        bool enabled;
        uint32_t lastAction;
        uint32_t timeout;
        char jobname[JOB_NAME_SIZE];
        char errormessage[JOB_ERROR_SIZE];
        uint32_t overloadCount;
        uint32_t jobId;

        uint32_t cycleDuration[JOB_CYCLE_SIZE];      // Timeouts in us
        uint32_t cycleCurrentLoad;                      // aktuelle Last in %

};



} /* namespace repraxi */ } /* namespace roboticcare */} // namespace de 





#endif
