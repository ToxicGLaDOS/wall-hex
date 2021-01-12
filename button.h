#ifndef BUTTON
#define BUTTON
#define BUTTON_PIN 4

#include "Arduino.h"

enum Event{NONE = 0, PRESS = 1, HOLD = 2, RELEASE = 3};

class Button {
    private:
        Event event = NONE;
        bool previousState = 0;
        unsigned long holdTimer = 0;
        unsigned long previousTime = 0;
    public:
        Button();
        void updateButton();
        Event getEvent(){return event;};
        unsigned long getHoldDuration(){return holdTimer;};
        void subtractHoldTime(unsigned long time){holdTimer -= time;};
        void press();
        void hold();
        void release();
        void none();
};

#endif