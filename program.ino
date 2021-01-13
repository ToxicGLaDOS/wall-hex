#include <FastLED.h>
#include "hex.h"
#include "button.h"

// How many leds in your strip?
#define NUM_LEDS 18*4

#define NUM_HEXES 4
#define DATA_PIN 3

// Define the array of leds
CRGB leds[NUM_LEDS];

uint8_t hex0indices[] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 69, 70, 71};
uint8_t hex1indices[] = {15, 16, 17, 18, 19, 20, 39, 40, 41, 60, 61, 62, 63, 64, 65, 66, 67, 68};
uint8_t hex2indices[] = {21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38};
uint8_t hex3indices[] = {42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59};

Hex hexes[NUM_HEXES] = {
  Hex(hex0indices),
  Hex(hex1indices),
  Hex(hex2indices),
  Hex(hex3indices)
};

Button button;

uint8_t selectedHex = 0;

enum OperationMode{NORMAL, SELECTING_HEX, SELECTING_PATTERN};

OperationMode operationMode = NORMAL;

unsigned long timeoutTimer = 0;
unsigned long previousTimeoutTime = 0;
const unsigned long timeoutDuration = 3000;


void setup() {
    Serial.begin(9600);
    Serial.println("Begin");
    pinMode(4, INPUT_PULLUP);
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
}

void loop() {
  for (int i = 0; i < NUM_HEXES; i++){
    Hex *hex = &hexes[i];

    hex->updateLights();
    CRGB* ledData = hex->getLedData();
    uint8_t* indices = hex->getIndices();
    uint8_t numLeds = hex->getNumLeds();


    for (int i = 0; i < numLeds; i++){
      leds[indices[i]] = ledData[i];
    }
  }
  FastLED.show();
  handleButton();
}

void handleButton() {
  button.updateButton();
  Event event = button.getEvent();
  unsigned long timeoutDelta = millis() - previousTimeoutTime;

  if (operationMode == NORMAL){
    // Any press sends you to SELECTING_HEX
    if(event == Event::PRESS || event == Event::LONG_PRESS){
      operationMode = SELECTING_HEX;
      hexes[selectedHex].beginSelectMode();
    }
  }
  else if(operationMode == SELECTING_HEX){
    if (event == Event::PRESS){
      hexes[selectedHex].endSelectMode();
      selectedHex++;
      selectedHex %= NUM_HEXES;
      hexes[selectedHex].beginSelectMode();
      timeoutTimer = 0;
    }
    else if(event == Event::LONG_PRESS){
      Serial.println("Transition to select pattern");
      operationMode = SELECTING_PATTERN;
      hexes[selectedHex].beginPatternSelectMode();
      timeoutTimer = 0;
    }
    else{
      timeoutTimer += timeoutDelta;
    }
    
    handleTimeout();
  }
  else if(operationMode == SELECTING_PATTERN){
    if (event == Event::PRESS){
      Serial.println("New pattern");
      hexes[selectedHex].changeMode();
      timeoutTimer = 0;
    }
    else if(event == Event::LONG_PRESS){
      Serial.println("Transition to select hex");
      operationMode = SELECTING_HEX;
      hexes[selectedHex].beginSelectMode();
      timeoutTimer = 0;
    }
    else{
      timeoutTimer += timeoutDelta;
    }

    handleTimeout();
  }
  previousTimeoutTime = millis();
}

void printEvent(){
  Event event = button.getEvent();
  switch(event){
    case Event::NONE:
      Serial.println(0);
      break;
    case Event::PRESS:
      Serial.println(1);
      break;
    case Event::HOLD:
      Serial.println(2);
      break;
    case Event::RELEASE:
      Serial.println(3);
      break;
  }
}

void handleTimeout(){
  if (timeoutTimer > timeoutDuration){
    Serial.println("Timeout");
    operationMode = NORMAL;
    hexes[selectedHex].endSelectMode();
    timeoutTimer = 0;
  }
}

