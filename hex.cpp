#include "hex.h"

Hex::Hex(uint8_t hexIndices[], uint8_t size) {
  for(int i = 0; i < size; i++) {
    indices[i] = hexIndices[i];
  }
  numLEDs = size;
}

void Hex::changeMode(){
  // Increment mode, but keep it in bounds for the mode functions array
  mode++;
  mode %= sizeof(modes)/sizeof(modes[0]);
  previousMode = mode; // This keeps endSelectMode from overwriting our changes
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

void Hex::beginPatternSelectMode(){
  mode = previousMode;
}

void Hex::updateLights(unsigned long delta){
  // Call the proper mode's function
  timer += delta;
  // This is some special sauce syntax for calling member functions that are data
  (this->*modes[mode])();
}


// MODE FUNCTIONS

void Hex::blinkRed(){
  CRGB red = CRGB::Red;
  if(timer > 500){
    if(ledData[0].r == red.r && ledData[0].g == red.g && ledData[0].g == red.g){
      for (int i = 0; i < numLEDs; i++){
        ledData[i] = CRGB::Black;
      }
    }
    else{
      for (int i = 0; i < numLEDs; i++){

        ledData[i] = red;
      }
    }
    timer = 0;
  }
}

void Hex::staticRed(){
  for (int i = 0; i < numLEDs; i++){
    ledData[i] = CRGB::Red;
  }
}

void Hex::staticGreen(){
  for (int i = 0; i < numLEDs; i++){
    ledData[i] = CRGB::Green;
  }
}

void Hex::staticBlue(){
  for (int i = 0; i < numLEDs; i++){
    ledData[i] = CRGB::Blue;
  }
}

void Hex::rainbowFade(){
  CRGB red = CRGB::Red;
  CRGB green = CRGB::Green;
  CRGB blue = CRGB::Blue;
  bool dimming = timer % 512 < 256;
  uint8_t brightness = dimming ? 255-(timer%256) : timer%256;


  uint8_t t1, t2;
  t1 = numLEDs/3;
  t2 = 2* numLEDs/3;
  for (int i = 0; i < t1; i++){
    CRGB color = red.lerp8(green, i/(float)t1*255);
    color.maximizeBrightness();
    color.fadeLightBy(brightness);
    ledData[i] = color;
  }
  for (int i = t1; i < t2; i++){
    CRGB color = green.lerp8(blue, i/(float)t2*255);
    color.maximizeBrightness();
    color.fadeLightBy(brightness);
    ledData[i] = color;
  }
  for (int i = t2; i < numLEDs; i++){
    CRGB color = blue.lerp8(red, i/(float)numLEDs*255);
    color.maximizeBrightness();
    color.fadeLightBy(brightness);
    ledData[i] = color;
  }
}

void Hex::rainbowSpin(){
  CRGB red = CRGB::Red;
  CRGB green = CRGB::Green;
  CRGB blue = CRGB::Blue;
  CRGB* baseColors = new CRGB(numLEDs);
  uint8_t t1, t2;

  const uint8_t millisTillMove = 50;
  uint8_t moves = timer/millisTillMove;


  t1 = numLEDs/3;
  t2 = 2* numLEDs/3;
  for (int i = 0; i < t1; i++){
    CRGB color = red.lerp8(green, i/(float)t1*255);
    color.maximizeBrightness();
    baseColors[i] = color;
  }
  for (int i = t1; i < t2; i++){
    CRGB color = green.lerp8(blue, i/(float)t2*255);
    color.maximizeBrightness();
    baseColors[i] = color;
  }
  for (int i = t2; i < numLEDs; i++){
    CRGB color = blue.lerp8(red, i/(float)numLEDs*255);
    color.maximizeBrightness();
    baseColors[i] = color;
  }
  for (int i = 0; i < numLEDs; i++){
    ledData[i] = baseColors[(i + moves) % numLEDs];
  }
  delete [] baseColors;
}

void Hex::rainbowSpinAndFade(){
  CRGB red = CRGB::Red;
  CRGB green = CRGB::Green;
  CRGB blue = CRGB::Blue;
  CRGB* baseColors = new CRGB(numLEDs);
  bool dimming = timer % 512 < 256;
  uint8_t brightness = dimming ? 255-(timer%256) : timer%256;
  uint8_t t1, t2;

  const uint8_t millisTillMove = 50;
  uint8_t moves = timer/millisTillMove;


  t1 = numLEDs/3;
  t2 = 2* numLEDs/3;
  for (int i = 0; i < t1; i++){
    CRGB color = red.lerp8(green, i/(float)t1*255);
    color.maximizeBrightness();
    color.fadeLightBy(brightness);    
    baseColors[i] = color;
  }
  for (int i = t1; i < t2; i++){
    CRGB color = green.lerp8(blue, i/(float)t2*255);
    color.maximizeBrightness();
    color.fadeLightBy(brightness);
    baseColors[i] = color;
  }
  for (int i = t2; i < numLEDs; i++){
    CRGB color = blue.lerp8(red, i/(float)numLEDs*255);
    color.maximizeBrightness();
    color.fadeLightBy(brightness);
    baseColors[i] = color;
  }
  for (int i = 0; i < numLEDs; i++){
    ledData[i] = baseColors[(i + moves) % numLEDs];
  }
  delete [] baseColors;
}

void Hex::chaserRed(){
  const uint8_t millisTillMove = 50;
  uint8_t moves = timer/millisTillMove;

  for (int i = 0; i < numLEDs; i++){
    ledData[i] = CRGB::Black;
  }
  ledData[(moves - 2) % numLEDs] = CRGB(255,0,0).fadeLightBy(255/2);
  ledData[(moves - 1) % numLEDs] = CRGB(255,0,0).fadeLightBy(255/4);
  ledData[(moves + 0) % numLEDs] = CRGB(255,0,0);
  ledData[(moves + 1) % numLEDs] = CRGB(255,0,0).fadeLightBy(255/4);
  ledData[(moves + 2) % numLEDs] = CRGB(255,0,0).fadeLightBy(255/2);
}

void Hex::chaserGreen(){
  const uint8_t millisTillMove = 50;
  uint8_t moves = timer/millisTillMove;

  for (int i = 0; i < numLEDs; i++){
    ledData[i] = CRGB::Black;
  }
  ledData[(moves - 2) % numLEDs] = CRGB(0,255,0).fadeLightBy(255/2);
  ledData[(moves - 1) % numLEDs] = CRGB(0,255,0).fadeLightBy(255/4);
  ledData[(moves + 0) % numLEDs] = CRGB(0,255,0);
  ledData[(moves + 1) % numLEDs] = CRGB(0,255,0).fadeLightBy(255/4);
  ledData[(moves + 2) % numLEDs] = CRGB(0,255,0).fadeLightBy(255/2);
}

void Hex::chaserBlue(){
  const uint8_t millisTillMove = 50;
  uint8_t moves = timer/millisTillMove;

  for (int i = 0; i < numLEDs; i++){
    ledData[i] = CRGB::Black;
  }
  ledData[(moves - 2) % numLEDs] = CRGB(0,0,255).fadeLightBy(255/2);
  ledData[(moves - 1) % numLEDs] = CRGB(0,0,255).fadeLightBy(255/4);
  ledData[(moves + 0) % numLEDs] = CRGB(0,0,255);
  ledData[(moves + 1) % numLEDs] = CRGB(0,0,255).fadeLightBy(255/4);
  ledData[(moves + 2) % numLEDs] = CRGB(0,0,255).fadeLightBy(255/2);
}

void Hex::vaporWaveFadeSpin(){
  CRGB pink = CRGB::Pink;
  CRGB blue = CRGB::Blue;
  CRGB lightBlue = CRGB::LightBlue;
  CRGB* baseColors = new CRGB(numLEDs);
  bool dimming = timer % 512 < 256;
  uint8_t brightness = dimming ? 255-(timer%256) : timer%256;
  uint8_t t1, t2;

  const uint8_t millisTillMove = 50;
  uint8_t moves = timer/millisTillMove;


  t1 = numLEDs/3;
  t2 = 2* numLEDs/3;
  for (int i = 0; i < t1; i++){
    CRGB color = pink.lerp8(blue, i/(float)t1*255);
    color.maximizeBrightness();
    color.fadeLightBy(brightness);    
    baseColors[i] = color;
  }
  for (int i = t1; i < t2; i++){
    CRGB color = blue.lerp8(lightBlue, i/(float)t2*255);
    color.maximizeBrightness();
    color.fadeLightBy(brightness);
    baseColors[i] = color;
  }
  for (int i = t2; i < numLEDs; i++){
    CRGB color = lightBlue.lerp8(pink, i/(float)numLEDs*255);
    color.maximizeBrightness();
    color.fadeLightBy(brightness);
    baseColors[i] = color;
  }
  for (int i = 0; i < numLEDs; i++){
    ledData[i] = baseColors[(i + moves) % numLEDs];
  }
  delete [] baseColors;
}

void Hex::pewPewPew(){
  unsigned long moveDelay = 50;
  unsigned long shootDelay = moveDelay * numLEDs * 2;
  unsigned long moves = timer / moveDelay; // Moves made by the shots
  unsigned long baseMoves = -(timer / shootDelay) % numLEDs; // Moves made by the shooter

  // Because generics is zero'd out, if we start the LEDs with
  // this patten the lights will all be (0 0 0) because baseMoves == generics[0] is true
  // so this block doesn't set the random light values. This will probably never matter
  // because it isn't possible to get the lights to this pattern when baseMoves is 0
  // so :shrug:
  if (baseMoves != generics[0]){
    generics[0] = baseMoves;
    generics[1] = random(0,255);
    generics[2] = random(0,255);
    generics[3] = random(0,255);
  }
  uint8_t r = generics[1];
  uint8_t g = generics[2];
  uint8_t b = generics[3];
  CRGB color = CRGB(r, g, b);
  color.maximizeBrightness();

  for (int i = 0; i < numLEDs; i++){
    ledData[i] = CRGB::Black;
  }
  ledData[baseMoves] = color;
  
  // moveDelay * numLEDs is how long it takes to go around
  // we subtract one moveDelay because we don't want to include
  // the movement that puts it over the shooter
  if (timer % shootDelay < moveDelay * numLEDs - moveDelay){
    CRGB copy = color;
    copy.fadeLightBy(255 - 255/3*3); // 100% brightness
    ledData[(moves + baseMoves + 1) % numLEDs] = copy;
  }
  if (timer % shootDelay > moveDelay * 1 && timer % shootDelay < moveDelay * numLEDs){
    CRGB copy = color;
    copy.fadeLightBy(255 - 255/3*2); // 66% brightness
    ledData[(moves + baseMoves - 0) % numLEDs] = copy;
  }
  if (timer % shootDelay > moveDelay * 2 && timer % shootDelay < moveDelay * numLEDs + moveDelay * 1){
    CRGB copy = color;
    copy.fadeLightBy(255 - 255/4*1); // 33% brightness
    ledData[(moves + baseMoves - 1) % numLEDs] = copy;
  }

}


