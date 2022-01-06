/*
Pink Sine Wave on 24 x 24 DotStar LED Matrix with input from microphone

  This code creates a sine wave on a 24 x 24 DotStar LED Matrix consisting of 9, 8 x 8 Matrices
  Includes input from microphone to control the wave.
*/


#include <Adafruit_GFX.h>
#include <Adafruit_DotStarMatrix.h>
#include <Adafruit_DotStar.h>


#define DATAPIN  12
#define CLOCKPIN 13


#define ONBOARDDATAPIN 8
#define ONBOARDCLOCKPIN 6


#define BRIGHTNESS 16


// Define matrix width and height.
#define mw 24
#define mh 24


#define TWO_PI 6.283185307179586476925286766559


Adafruit_DotStar onboardDotStar(1, ONBOARDDATAPIN, ONBOARDCLOCKPIN, DOTSTAR_BRG);
Adafruit_DotStarMatrix matrix = Adafruit_DotStarMatrix(
                                  (uint8_t)8, (uint8_t)8, 3, 3, DATAPIN, CLOCKPIN,
                                  DS_TILE_TOP   + DS_TILE_LEFT   + DS_TILE_ROWS   + DS_TILE_PROGRESSIVE +
                                  DS_MATRIX_TOP + DS_MATRIX_LEFT + DS_MATRIX_COLUMNS + DS_MATRIX_PROGRESSIVE,
                                  DOTSTAR_BGR);


uint32_t magenta = matrix.Color(255, 0, 255);


// MICROPHONE INPUT (Code based on "Callibration" Analog example in Arduino)
const int micPin = A0;    // pin that the mic is attached to


// variables:
int micValue = 0;         // the mic value
int micMin = 1023;        // min
int micMax = 0;           // max
