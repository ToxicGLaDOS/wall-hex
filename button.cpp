#include "button.h"

Button::Button(){}

void Button::updateButton(){
    bool pressed = digitalRead(BUTTON_PIN);
    switch (event) {
    case NONE:
        if(pressed){
            press();
        }
        break;
    case PRESS:
        if(pressed){
            hold();
        } else{
            release();
        }
        break;
    case HOLD:
        if(pressed){
            hold();
        } else{
            release();
        }
        break;
    case RELEASE:
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
    event = PRESS;
    holdTimer = 0;
    previousTime = millis();
}

void Button::hold(){
    event = HOLD;
    unsigned long delta = millis() - previousTime;
    holdTimer += delta;
    previousTime = millis();
}

void Button::release(){
    event = RELEASE;
}

void Button::none(){
    event = NONE;
    holdTimer = 0;
}


