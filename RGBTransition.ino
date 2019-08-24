#include <Adafruit_NeoPixel.h>

/*
* Code for cross-fading 3 LEDs, red, green and blue (RGB) 
* To create fades, you need to do two things: 
*  1. Describe the colors you want to be displayed
*  2. List the order you want them to fade in
*
* DESCRIBING A COLOR:
* A color is just an array of three percentages, 0-100, 
*  controlling the red, green and blue LEDs
*
* Red is the red LED at full, blue and green off
*   int red = { 100, 0, 0 }
* Dim white is all three LEDs at 30%
*   int dimWhite = {30, 30, 30}
* etc.
*
* Some common colors are provided below, or make your own
* 
* LISTING THE ORDER:
* In the main part of the program, you need to list the order 
*  you want to colors to appear in, e.g.
*  crossFade(red);
*  crossFade(green);
*  crossFade(blue);
*
* Those colors will appear in that order, fading out of 
*    one color and into the next  
*
* In addition, there are 5 optional settings you can adjust:
* 1. The initial color is set to black (so the first color fades in), but 
*    you can set the initial color to be any other color
* 2. The internal loop runs for 1020 interations; the 'wait' variable
*    sets the approximate duration of a single crossfade. In theory, 
*    a 'wait' of 10 ms should make a crossFade of ~10 seconds. In 
*    practice, the other functions the code is performing slow this 
*    down to ~11 seconds on my board. YMMV.
* 3. If 'repeat' is set to 0, the program will loop indefinitely.
*    if it is set to a number, it will loop that number of times,
*    then stop on the last color in the sequence. (Set 'return' to 1, 
*    and make the last color black if you want it to fade out at the end.)
* 4. There is an optional 'hold' variable, which pasues the 
*    program for 'hold' milliseconds when a color is complete, 
*    but before the next color starts.
* 5. Set the DEBUG flag to 1 if you want debugging output to be
*    sent to the serial monitor.
*
*    The internals of the program aren't complicated, but they
*    are a little fussy -- the inner workings are explained 
*    below the main loop.
*
* April 2007, Clay Shirky <clay.shirky@nyu.edu> 
*/ 

#define DEBUG       1     // DEBUG counter; if set to 1, will write values back via serial
#define PIN         30
#define HOLD        0     // Optional hold when a color is complete, before the next crossFade
#define REPEATLOOP  1     // How many times should we loop before stopping? (0 for no stop)
#define TIME        1000  // time in ms
#define STEPS       100
#define WAIT        500    // ms internal crossFade delay; increase for slower fades


//NeoPixel configuration.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN);

static uint8_t startR = 0, startG = 0, startB = 0;

int j = 0;          // Loop counter for repeat


// Set up the LED outputs
void setup(){
  strip.begin();
  //strip.setBrightness(64);
  strip.show(); // Initialize all pixels to 'off'

  if (DEBUG) {           // If we want to see values for debugging...
    Serial.begin(115200);  // ...set up the serial ouput 
  }
}

// Main program: list the order of crossfades
void loop(){
  crossFade(0xFF, 0x00, 0x00);
  crossFade(0x00, 0xFF, 0x00);

  if (REPEATLOOP) { // Do we loop a finite number of times?
    j += 1;
    if (j >= REPEATLOOP) { // Are we there yet?
      exit(j);         // If so, stop.
    }
  }
}

/* crossFade() converts the percentage colors to a 
*  0-255 range, then loops 1020 times, checking to see if  
*  the value needs to be updated each time, then writing
*  the color values to the correct pins.
*/

void crossFade(uint8_t targetR, uint8_t targetG, uint8_t targetB) {
// Initialize color variables


  float curR = (float)startR, 
        curG = (float)startG, 
        curB = (float)startB;


  float stepR = ((float)(targetR - startR) / STEPS);
  float stepG = ((float)(targetG - startG) / STEPS);
  float stepB = ((float)(targetB - startB) / STEPS);


  if(DEBUG){
      Serial.print("Target RGB: ");
      Serial.print((uint8_t)targetR, HEX);
      Serial.print(":");
      Serial.print((uint8_t)targetG, HEX);
      Serial.print(":");
      Serial.println((uint8_t)targetB, HEX);
      Serial.print("Start RGB: ");
      Serial.print(startR, HEX);
      Serial.print(":");
      Serial.print(startG, HEX);
      Serial.print(":");
      Serial.println(startB, HEX);
      Serial.print("Step: ");
      Serial.print((uint8_t)stepR);
      Serial.print(":");
      Serial.print((uint8_t)stepG);
      Serial.print(":");
      Serial.println((uint8_t)stepB);
      Serial.print("Step: ");
      Serial.print(stepR);
      Serial.print(":");
      Serial.print(stepG);
      Serial.print(":");
      Serial.println(stepB);

    }

  strip.setPixelColor(0, (uint8_t)curR, (uint8_t)curG, (uint8_t)curB);
  strip.show();
  delay(WAIT); // Pause for 'wait' milliseconds before resuming the loop

  for (int i = 0; i < STEPS; i++) {
    curR += stepR;
    curG += stepG;
    curB += stepB;

    strip.setPixelColor(0, (uint8_t)curR, (uint8_t)curG, (uint8_t)curB);
    strip.show();
    delay(TIME/STEPS); // Pause for 'wait' milliseconds before resuming the loop

    if (DEBUG) { // If we want serial output, print it at the 
      Serial.print("Loop/RGB: #");
      Serial.print(i + 2);
      Serial.print(" | ");
      Serial.print((uint8_t)curR, HEX);
      Serial.print(" / ");
      Serial.print((uint8_t)curG, HEX);
      Serial.print(" / ");  
      Serial.println((uint8_t)curB, HEX); 
    }
  }
  
  // Update current values for next loop
  startR = targetR; 
  startG = targetG; 
  startB = targetB;
  delay(HOLD); // Pause for optional 'wait' milliseconds before resuming the loop
}
