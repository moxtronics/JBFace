#include "FastLED.h"
#include "faces.h"


// Matrix size
#define NUM_ROWS 22
#define NUM_COLS 22

// Define pins
#define DATA_PIN A0


#define smileButton 2

//smile timing stuff
//******************

byte smileButtonState;



//*****************

// look direction control
#define LOOK_SLIDER A1

int outputVal = 0;

// LED brightness
#define BRIGHTNESS 8

// Define the array of leds
#define NUM_LEDS NUM_ROWS * NUM_COLS
CRGB leds[NUM_LEDS];

// Animation controls
byte pressed = 1; // Tracks which face is currently being displayed

bool buttonSmilePressed = false;
int oldstate = HIGH;

bool sliderValue = false;

// *************** SETUP ***************

void setup() {
 
  pinMode(smileButton, INPUT_PULLUP);

  pinMode(LOOK_SLIDER, INPUT);
  Serial.begin(9600);

  //GRB order for 22x matrix
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  mid(); // Start the display with the forward-facing face
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
    case 3: smile();   break;
  }
}

void smile() {
  // Read the forward face from PROGMEM, then display it.
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = pgm_read_dword_near(Smile + i);
  }
  FastLED.show();
  delay(3000);

 
}

void midB() {
  // Read the right face from PROGMEM, then display it.
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = pgm_read_dword_near(IdleBlink + i);
  }
  FastLED.show();
  // Hold the blink for 50 milliseconds so the blink is visible
  delay(120);
}
// END of face functions ***********************

// *************** LOOP ******************

void loop() {
  
  // Read the button inputs
  buttonSmilePressed = digitalRead(smileButton) == LOW;

  //control look direction with potentiometer
  int sliderValue = analogRead(LOOK_SLIDER);
  // map function values: map(value, fromLow, fromHigh, toLow, toHigh)
  outputVal = map(sliderValue, 0, 800, 1, 3); 
  Serial.println(sliderValue);
  delay(100);

  if (outputVal == 1) //look left
  {
    
    left();
  }
  else if (outputVal == 2) // look mid
  {
    
    mid();
  }
  else if (outputVal == 3) // look right
  {
    
    right();
  }

  // Blink the eyes ocassionally
  EVERY_N_MILLISECONDS_I(blinktime, 3000) {
    blinking();
    // After blinking, return to the previous face
    switch (pressed) {
      case 0: left(); break;
      case 1: mid(); break;
      case 2: right(); break;
    }
    // Set the next blink delay
    blinktime.setPeriod( random16(1000, 3000) );
  }

} //END of loop()

//*****************************************************************************
