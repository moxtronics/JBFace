#include "FastLED.h"
#include "faces.h"


// Matrix size
#define NUM_ROWS 22
#define NUM_COLS 22

// Define pins
#define DATA_PIN 3

//#define BUTTON_0 9
//#define BUTTON_1 10
//#define BUTTON_2 11
#define smileButton 8

//smile timing stuff
//******************
#define PUSHED HIGH //switch pressed gives HIGH (+5V) on pin

unsigned long smileMillis;
unsigned long switchMillis;
unsigned long lastMillis;
unsigned long delayTime;

const unsigned long smileDelay = 500; //1/2 second
const unsigned long debounceDelay  = 50;  //50ms

#define DELAY 3000 //milliseconds

static unsigned long time;
static unsigned int state;
static unsigned int button;
static unsigned int faceDelay;



byte smileButtonState

//*****************

// look direction control
//left middle right values: 0-340, 341-680, 681-1024
#define LOOK_SLIDER A0



// LED brightness
#define BRIGHTNESS 180

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

  time = 0;
  state = 1;
 
  pinMode(smileButton, INPUT_PULLUP);

  pinMode(LOOK_SLIDER, INPUT);
  Serial.begin(115200);


  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
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
  //delay(3000);

 
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
    buttonSmilePressed = digitalRead(smileButton);

    switch state
    {
    case 1: 
            if(buttonSmilePressed == HIGH) { // enter next state and save current time
            state = 2;

            mid();
            blinking();


            time = millis();
        }
        break;
    case 2: 
        if(buttonSmilePressed == HIGH) // reset timer

          smile();

            time = millis();
        if(time - millis() > DELAY) //check if time has passed
            state = 1;


        break;
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
  }
  else if (outputVal == 3) // look right
  {
    
    right();
  }

  // Blink the eyes ocassionally
  EVERY_N_MILLISECONDS_I(blinktime, 1000) {
    blinking();
    
    // After blinking, return to the previous face
    /*switch (pressed) {
      case 0: left(); break;
      case 1: mid(); break;
      case 2: right(); break;
    }
    */

    // Set the next blink delay
    blinktime.setPeriod( random16(1000, 3000) );
  }

} //END of loop()

//*****************************************************************************
