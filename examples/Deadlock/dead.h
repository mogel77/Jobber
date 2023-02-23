#include "Job.h" 



class Deadlock : public de::roboticcare::github::Job {

    public:
        Deadlock(uint8_t pin);

    protected:
        virtual void execute(bool firstrun) override;

    private:
        uint8_t pin;

};
