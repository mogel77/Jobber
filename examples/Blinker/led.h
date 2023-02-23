#include "Job.h" 



class LED : public de::roboticcare::github::Job {

    public:
        LED(uint8_t pin = LED_BUILTIN, uint16_t timeout = 1000);

    protected:
        virtual void execute(bool firstrun) override;

    private:
        uint8_t pin;
        bool toggle;

};
