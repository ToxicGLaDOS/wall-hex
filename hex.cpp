#include "hex.h"

Hex::Hex(uint8_t hexIndices[]) {
  for(int i = 0; i < sizeof(indices); i++) {
    indices[i] = hexIndices[i];
  }
}

void Hex::changeMode(){
  // Increment mode, but keep it in bounds for the mode functions array
  mode++;
  mode %= sizeof(modes);
}

void Hex::beginSelectMode(){
  selectMode = true;
  previousMode = mode;
  mode = 0;
}

void Hex::endSelectMode(){
  // This guard is here so we don't go back to a previous mode by calling this erronously
  if(selectMode){
    selectMode = false;
    mode = previousMode;
  }
}

void Hex::updateLights(){
  // Call the proper mode's function

  unsigned long delta = millis() - previousTime;
  timer += delta;
  // This is some special sauce syntax for calling member functions that are data
  (this->*modes[mode])();

  previousTime = millis();

}


// MODE FUNCTIONS

void Hex::blinkRed(){
  CRGB red = CRGB::Red;
  if(timer > 500){
    if(ledData[0].r == red.r && ledData[0].g == red.g && ledData[0].g == red.g){
      for (int i = 0; i < sizeof(ledData)/sizeof(ledData[0]); i++){
        ledData[i] = CRGB::Black;
      }
    }
    else{
      for (int i = 0; i < sizeof(ledData)/sizeof(ledData[0]); i++){

        ledData[i] = red;
      }
    }
    timer = 0;
  }

}

void Hex::staticRed(){
  for (int i = 0; i < sizeof(ledData)/sizeof(ledData[0]); i++){
    ledData[i] = CRGB::Red;
  }

}
