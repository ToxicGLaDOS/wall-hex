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

enum OperationMode{NORMAL, SELECTING};

OperationMode operationMode = NORMAL;

void setup() {
    Serial.begin(9600);
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
  if (event == PRESS){
    // Do nothing. Wait for the release
  }
  else if (event == HOLD){
    operationMode = SELECTING;
    unsigned long holdTime = button.getHoldDuration();
    if (holdTime > 500){
      operationMode = SELECTING;
      button.subtractHoldTime(500);
      hexes[selectedHex].endSelectMode();
      selectedHex++;
      selectedHex %= NUM_HEXES;
      hexes[selectedHex].beginSelectMode();
    }
  }
  else if (event == RELEASE){
    operationMode = NORMAL;
    hexes[selectedHex].endSelectMode();
  }
}

