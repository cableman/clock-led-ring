#include "my_wifi.h"

#include <ezTime.h>
#include <FastLED.h>

#define DATA_PIN D3
#define NUM_LEDS 60

CRGB leds[NUM_LEDS];

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
  leds[hour.toInt()*5] = CRGB::Green;

  String min = myTZ.dateTime("i");
  leds[min.toInt()] = CRGB::Blue;

  String sec = myTZ.dateTime("s");
  leds[sec.toInt()] = CRGB::Red;

  FastLED.show();
  FastLED.delay(1000);
}

void ledTest() {
  for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
      // Turn our current led on to white, then show the leds
      leds[whiteLed] = CRGB::Blue;
 
      // Show the leds (only one of which is set to white, from above)
      FastLED.show();
 
      // Wait a little bit
      FastLED.delay(100);
 
      // Turn our current led back to black for the next loop around
      leds[whiteLed] = CRGB::Black;
   }
}

void setup() {
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(100);

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(40);

  // Connect to WIFI.
  MyWifi* myWifi = new MyWifi(leds);
  myWifi->connect();

  // Connect to time servers.
  connectTime();
}

void loop() {
  if (runTest) {
    ledTest();
    runTest = false;
  }
  simpleTime();
}
