#include "Job.h" 



class ALive : public de::roboticcare::github::Job {

    public:
        ALive(uint8_t pin = LED_BUILTIN);

    protected:
        virtual void execute(bool firstrun) override;

    private:
        uint8_t pin;
        bool toggle;

};
