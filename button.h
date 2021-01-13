#ifndef BUTTON
#define BUTTON
#define BUTTON_PIN 4

#include "Arduino.h"

enum class Event{NONE = 0, PRESS = 1, HOLD = 2, RELEASE = 3, LONG_PRESS = 4};
enum class State{NONE = 0, PRESS = 1, HOLD = 2, RELEASE = 3};

class Button {
    private:
        const static unsigned long doublePressLength = 250;
        const static unsigned long longPressLength = 750;
        Event event = Event::NONE;
        State state = State::NONE;
        bool longPressedAlreadyFired = false;
        unsigned long previousTime = 0;
        unsigned long lastPressTime = 0;
        bool isPressed();
    public:
        Button();
        void updateButton();
        Event getEvent(){return event;};
        void press();
        void hold();
        void release();
        void none();
};

#endif