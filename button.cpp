#include "button.h"

Button::Button(){}

void Button::updateButton(){
    event = Event::NONE;
    bool pressed = isPressed();
    switch (state) {
    case State::NONE:
        if(pressed){
            press();
        }
        break;
    case State::PRESS:
        if(pressed){
            hold();
        } else{
            release();
        }
        break;
    case State::HOLD:
        if(pressed){
            hold();
        } else{
            release();
        }
        break;
    case State::RELEASE:
        if(pressed){
            press();
        } else{
            none();
        }
        break;
    default:
        break;
    }
}


void Button::press(){
    state = State::PRESS;
    previousTime = millis();
    lastPressTime = millis();
}

void Button::hold(){
    if (!longPressedAlreadyFired && millis() - lastPressTime >= longPressLength){
        event = Event::LONG_PRESS;
        longPressedAlreadyFired = true;
    }
    else{
        state = State::HOLD;
    }
}

void Button::release(){
    if (millis() - lastPressTime < longPressLength){
        event = Event::PRESS;
    }
    state = State::RELEASE;
    longPressedAlreadyFired = false;
}

void Button::none(){
    state = State::NONE;
}

bool Button::isPressed(){
    return !digitalRead(BUTTON_PIN);
}

