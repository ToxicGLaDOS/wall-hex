#ifndef WALL_HEX
#define WALL_HEX
#include <FastLED.h>

class Hex {
  private:
    typedef void (Hex::*hex_func) ();
    uint8_t indices[18];  // The indices in the global led strip which this hex uses
    CRGB ledData[18]; // The rgb values for each led in the hex
    uint8_t mode = 1; // The current display mode for this hex
    unsigned long previousTime = 0; // Used by updateLights to update timer
    unsigned long timer = 0;  // Used by hex functions to keep track of timings
    bool selectMode = false;
    uint8_t previousMode = 0; // Used by endSelectMode to go back to the previous mode
    // List of mode functions
    hex_func modes[2] = {&blinkRed, &staticRed};

  public:
    Hex(uint8_t hexIndices[]);
    void changeMode();

    void beginSelectMode();

    void endSelectMode();

    void updateLights();

    void blinkRed();

    void staticRed();

    uint8_t getNumLeds(){return sizeof(indices);};

    uint8_t* getIndices(){return indices;};

    CRGB* getLedData(){return ledData;};

};

#endif
