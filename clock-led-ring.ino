#include "my_wifi.h"

#include <ezTime.h>
#include <FastLED.h>

#define DATA_PIN D3
#define NUM_LEDS 60

CRGB leds[NUM_LEDS];

int brightness = 80;
int hourSteps = NUM_LEDS/12;
bool runTest = true;

/**
 * Connect to NTP time server.
 */
Timezone myTZ;
void connectTime() 
{
  FastLED.delay(1000);

  myTZ.setLocation("Europe/Copenhagen");
  waitForSync();
}

/**
 * Simple time.
 */
void simpleTime()
{
  FastLED.clear();

  String hour = myTZ.dateTime("h");
  int current = hour.toInt()*hourSteps;
  leds[current ? current -1 : 0] = CRGB::Green;

  String min = myTZ.dateTime("i");
  leds[min.toInt()] = CRGB::Blue;

  String sec = myTZ.dateTime("s");
  leds[sec.toInt()] = CRGB::Red;

  FastLED.show();
  FastLED.delay(1000);
}

/**
 * Test that leds are working.
 */
void ledTest() {
  for(int currentLed = 0; currentLed < NUM_LEDS; currentLed = currentLed + 1) {
      // Turn our current led on to white, then show the leds
      leds[currentLed] = CRGB::Blue;
 
      // Show the leds (only one of which is set to white, from above)
      FastLED.show();
 
      // Wait a little bit
      FastLED.delay(100);
 
      // Turn our current led back to black for the next loop around
      leds[currentLed] = CRGB::Black;
   }
}

/**
 * Setup leds, connect to wifi and get NTP time.
 */
void setup() {
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(100);

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(brightness);

  // Connect to WIFI.
  MyWifi* myWifi = new MyWifi(leds);
  myWifi->connect();

  // Connect to time servers.
  connectTime();
}

/**
 * Run the show.
 */
void loop() {
  if (runTest) {
    // Run led test the first time to light up evert led (check that they work).
    ledTest();
    runTest = false;
  }
  simpleTime();
}
