#include "FastLED.h"
#include "faces.h"
#include "SafeString.h"


// Matrix size
#define NUM_ROWS 22
#define NUM_COLS 22

// Define pins
#define DATA_PIN 3

#define smileButton 8
#define sparkleButton 9

//smile timing stuff
//******************
//Basic single shot delay (1/1 replaces regular delay)

unsigned long DELAY_TIME = 3000; // 3 sec
unsigned long delayStart = 0; // the time the delay started
bool delayRunning = false; // true if still waiting for delay to finish

byte smileButtonState;

//*****************

// look direction control
//left middle right values: 0-340, 341-680, 681-1024
#define LOOK_SLIDER A0



// LED brightness
#define BRIGHTNESS 250

// Define the array of leds
#define NUM_LEDS NUM_ROWS * NUM_COLS
CRGB leds[NUM_LEDS];

// Animation controls
byte pressed = 1; // Tracks which face is currently being displayed

bool buttonSmilePressed = false;
bool buttonSparklePressed = false;
int oldstate = HIGH;

bool sliderValue = false;

// *************** SETUP ***************


void setup() {

  //delay stuff
  delayStart = millis();
  delayRunning = true;

  buttonSmilePressed = digitalRead(smileButton) == LOW;
  pinMode(smileButton, INPUT_PULLUP);

  buttonSparklePressed = digitalRead(sparkleButton) == LOW;
  pinMode(sparkleButton, INPUT_PULLUP);

  pinMode(LOOK_SLIDER, INPUT);
  Serial.begin(9600);


  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  mid(); // Start the display with the forward-facing face
}

//smile timer
void checkSmile() {
  // check if delay has timed out
  if (delayRunning && ((millis() - delayStart) >= DELAY_TIME)) 
  {
    delayRunning = false; // finished delay -- single shot, once only
    // returns to default face, same as mid()function
     for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = pgm_read_dword_near(IdleFace + i);
  }
  FastLED.show();

  }
}


// face functions: ****************
void mid() {
  // Read the forward face from PROGMEM, then display it.
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = pgm_read_dword_near(IdleFace + i);
  }
  FastLED.show();
}

void left() {
  // Read the left face from PROGMEM, then display it.
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = pgm_read_dword_near(LeftFace + i);
  }
  FastLED.show();
}

void right() {
  // Read the right face from PROGMEM, then display it.
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = pgm_read_dword_near(RightFace + i);
  }
  FastLED.show();
}

void blinking() {
  // Pick the appropriate blinking face based on the current facing
  switch (pressed) {
    //case 0: leftB();   break;
    case 1: midB();    break;
    //case 2: rightB();  break;
    //case 3: smile();   break;
  }
}

void smile() {
  // Read the forward face from PROGMEM, then display it.
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = pgm_read_dword_near(Smile + i);
  }
  FastLED.show();
}

void sparkle() {
// Read the forward face from PROGMEM, then display it.
// 3 frames animation
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = pgm_read_dword_near(Sparkle1 + i);
  }
  FastLED.show();
  delay(200);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = pgm_read_dword_near(Sparkle2 + i);
  }
  FastLED.show();
  delay(200);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = pgm_read_dword_near(Sparkle3 + i);
  }
  FastLED.show();
  delay(200);

}

void midB() {
  // Read the right face from PROGMEM, then display it.
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = pgm_read_dword_near(IdleBlink + i);
  }
  FastLED.show();
  // Hold the blink for 50 milliseconds so the blink is visible
  delay(50);
}
// END of face functions ***********************

// no blinks when looking left or right

// *************** LOOP ******************

void loop() {

    // Read the button!

    //checkSmile();
    if (digitalRead(smileButton) == LOW) {
    
    smile();
  }
    if (digitalRead(sparkleButton) == LOW) {
    
    sparkle();
  }

  //control look direction with potentiometer
  int sliderValue = analogRead(LOOK_SLIDER);
  // map function values: map(value, fromLow, fromHigh, toLow, toHigh)

  int outputVal = 0;
  outputVal = map(sliderValue, 0, 1023, 1, 3); 
  Serial.println(sliderValue);
  delay(100);

  if (outputVal == 1) //look left
  {
    
    left();
  }
  else if (outputVal == 2) // look mid
  {
    
    mid();

    EVERY_N_MILLISECONDS_I(blinktime, 1500) {
    blinking();
    blinktime.setPeriod( random16(1000, 4000) );
    }
  }
  else if (outputVal == 3) // look right
  {
    
    right();
  }

  // Blink the eyes ocassionally
  //EVERY_N_MILLISECONDS_I(blinktime, 1000) {
   // blinking();
    
    // After blinking, return to the previous face
    /*switch (pressed) {
      case 0: left(); break;
      case 1: mid(); break;
      case 2: right(); break;
    }
    */

    // Set the next blink delay
    //blinktime.setPeriod( random16(1000, 3000) );
  //}

} //END of loop()

//*****************************************************************************
