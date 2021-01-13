#ifndef WALL_HEX
#define WALL_HEX
#include <FastLED.h>

class Hex {
  private:
    typedef void (Hex::*hex_func) ();
    uint8_t indices[50];  // The indices in the global led strip which this hex uses
    CRGB ledData[50]; // The rgb values for each led in the hex
    uint8_t mode = 11; // The current display mode for this hex
    unsigned long timer = 0;  // Used by hex functions to keep track of timings
    bool selectMode = false;
    uint8_t numLEDs = 0;
    uint8_t previousMode = 0; // Used by endSelectMode to go back to the previous mode
    uint8_t generics[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // 8 Generic variables to be used by light functions
    // List of mode functions
    hex_func modes[12] = {&blinkRed, &staticRed, &staticGreen, &staticBlue, &rainbowFade,
                         &rainbowSpin, &rainbowSpinAndFade, &chaserRed, &chaserGreen, &chaserBlue,
                         &vaporWaveFadeSpin, &pewPewPew};

  public:
    Hex(uint8_t hexIndices[], uint8_t size);
    void changeMode();

    void beginSelectMode();

    void endSelectMode();

    void beginPatternSelectMode();

    void updateLights(unsigned long delta);

    void blinkRed();

    void staticRed();

    void staticGreen();
    
    void staticBlue();

    void rainbowFade();

    void rainbowSpin();

    void rainbowSpinAndFade();
    
    void chaserRed();

    void chaserGreen();

    void chaserBlue();

    void vaporWaveFadeSpin();

    void pewPewPew();

    uint8_t getNumLeds(){return numLEDs;};

    uint8_t* getIndices(){return indices;};

    CRGB* getLedData(){return ledData;};

};

#endif
