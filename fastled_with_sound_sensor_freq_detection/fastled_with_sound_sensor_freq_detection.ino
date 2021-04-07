#include <FastLED.h>
/*
 * Samra Kasim
 * Computer Architecture EN605.204
 * Arduino sound detection project with FastLED and sound sensor
 * April 5, 2021
*/
FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

// FastLED definitions
#define NUM_LEDS 74
#define LED_PIN 7
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
#define MAX_BRIGHTNESS 164
#define MIN_BRIGHTNESS 32
#define PURPLE 192
#define BLUE 169
#define GREEN 96
#define YELLOW 64
#define ORANGE 32
#define RED 0
#define PINK 224
#define AQUA 128

// sound sensor definitions
#define SOUND_PIN 8
#define SAMPLING_WINDOW 10

const int HALF_STRIP = NUM_LEDS/2;
int sensorRead;
int gHue;
unsigned long millisCurrent;
unsigned long millisLast;
unsigned long millisElapsed;
int soundDetected;

void setup() {
  // put your setup code here, to run once:
  delay(3000);  // sanity check to ensure nothing is broken at start up
  Serial.begin(9600);
  FastLED.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(MIN_BRIGHTNESS);
  gHue = 0;
  sensorRead = 0;
  millisCurrent;
  millisLast = 0;
  millisElapsed = 0;
  soundDetected = 0;
}

void ledTraverse(int leftHue, int rightHue, int delayMillis) {
  // splits LED strip in half and enables travels back and forth on each side of strip using different colors

    for(int i = 0; i < HALF_STRIP-1 / 2; i++) {
  
      leds[i] = CHSV(leftHue, 255, 255);
      leds[NUM_LEDS-i] = CHSV(rightHue, 255, 255);
      FastLED.show();
      leds[i] = CRGB::Black; // clear led to give appearance of a track
      leds[NUM_LEDS-i] = CRGB::Black;
      delay(delayMillis);      
  }

    for(int i = HALF_STRIP-1; i > 0; i--) {

      leds[i] = CHSV(leftHue, 255, 255);
      leds[NUM_LEDS - i] = CHSV(rightHue, 255, 255);
      FastLED.show();
      leds[i] = CRGB::Black; // clear led to give appearance of a track
      leds[NUM_LEDS - i ] = CRGB::Black;
      delay(delayMillis);      
  }
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
  leds[37] = CRGB::Black;
}

void addGlitter(fract8 chanceOfGlitter) 
{
  // built-in FastLED glitter effect
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void invokeLed(int soundDetected) {
  // depending on the sound frequency detected, the color and brightness of the individual changes
    if (soundDetected < 1) {
    FastLED.setBrightness(MIN_BRIGHTNESS);
    fill_solid(leds, NUM_LEDS, CRGB::Purple);
  }

  if (0 < soundDetected && soundDetected < 21) {
    FastLED.setBrightness(32);
    rainbowWithGlitter();
    ledTraverse(PINK, AQUA, 10);
    ledTraverse(AQUA, PINK, 10);  
  }

    if (21 < soundDetected && soundDetected < 101) {
    FastLED.setBrightness(68);
    rainbowWithGlitter();
    ledTraverse(PURPLE, BLUE, 10);
    ledTraverse(BLUE, PURPLE, 10);
  }
  
  if (101 < soundDetected && soundDetected < 201) {
    FastLED.setBrightness(128);
    rainbowWithGlitter();
    ledTraverse(GREEN, YELLOW, 10);
    ledTraverse(YELLOW, GREEN, 10);  
  }

  if (soundDetected > 350) {
    FastLED.setBrightness(MAX_BRIGHTNESS);
    rainbowWithGlitter();
    ledTraverse(ORANGE, RED, 10);
    ledTraverse(RED, ORANGE, 10);
  }
}

void loop() {
  /*
   * frequency detection algorithm is from https://github.com/BasOnTech/Arduino-Beginners-EN/tree/master/E30-sound-sensor
   * the arduino sound sensor emits a digital output 1 when no noise is deteched and 0 when noise is detected
   * the following adds the number of times sounds is detected within a sampling window
   * the invokeLed() method is then used to assign values for the FastLed display
  */
  millisCurrent = millis();
  millisElapsed = millisCurrent - millisLast;

  sensorRead = digitalRead(SOUND_PIN);

  if (sensorRead == LOW) {
    soundDetected++;
  }
    
  if (millisElapsed > SAMPLING_WINDOW) {
    Serial.println(soundDetected);
    invokeLed(soundDetected);
    soundDetected = 0;
    millisLast = millisCurrent;
  } 
}
