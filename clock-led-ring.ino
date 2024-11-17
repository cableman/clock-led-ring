#include "my_wifi.h"

#include <ezTime.h>
#include <FastLED.h>

#define DATA_PIN D3
#define NUM_LEDS 60
#define DEBUG false

CRGB leds[NUM_LEDS];

int offset = 30;
int brightness = 160;
int indicatorBrightness = 96;
int hourSteps = NUM_LEDS/12;

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
 * Set brightness for single led.
 */
void setLedBrightness(int index, int value = indicatorBrightness)
{
  CHSV _targetHSV = rgb2hsv_approximate(leds[index]);

  _targetHSV.v = value;
  leds[index] = _targetHSV;
}

/**
 * Light up every 15 min.
 */
void lightUpQuater()
{
  for (int i; i <= NUM_LEDS; i++) {
    if (i % 15 == 0) {
      leds[i] = CRGB::Yellow;
      setLedBrightness(i);
    }
  }
}


/**
 * Light up every hour
 */
void lightUpHour()
{
  for (int i; i <= NUM_LEDS; i++) {
    if (i % hourSteps == 0) {
      leds[i] = CRGB::Yellow;
      setLedBrightness(i);
    }
  }
}

/**
 * Calculate offset based on where you first led is placed.
 */
int calculateOffset(int led)
{
  int n = led + offset;
  if (n >= NUM_LEDS)  {
    n = n - NUM_LEDS;
  }

  return n;
}

/**
 * Display time.
 */
void showTime(int hour, int min, int sec)
{
  FastLED.clear();
  //lightUpQuater();

  // Hours
  int hourLed = calculateOffset(hour*hourSteps);
  leds[hourLed] = CRGB::Green;
  Serial.println((String)"Hours: " + hour + ", Led: " + hourLed);

  // Min
  int minLed = calculateOffset(min);
  leds[minLed] = CRGB::Blue;
  Serial.println((String)"Min: " + min + ", Led: " + minLed);

  // Seconds
  int secLed = calculateOffset(sec);
  leds[secLed] = CRGB::Red;
  Serial.println((String)"Sec: " + sec + ", Led: " + secLed);
  Serial.println("-------------------");

  FastLED.show();
  FastLED.delay(1000);
}

/**
 * Simple time.
 */
void simpleTime()
{
  String hour = myTZ.dateTime("h");
  String min = myTZ.dateTime("i");
  String sec = myTZ.dateTime("s");

  showTime(hour.toInt(), min.toInt(), sec.toInt());
}

/**
 * Test that leds are working.
 */
void ledTest(CRGB color) {
  for(int currentLed = 0; currentLed < NUM_LEDS; currentLed = currentLed + 1) {
      // Turn our current led on to white, then show the leds
      leds[currentLed] = color;
 
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

  Serial.begin(9600);
  Serial.println("Connected...");

  // Init leds.
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);

  // Run led test the first time to light up every led (check that they work).
  Serial.println("Testing leds....");
  FastLED.setBrightness(255);
  ledTest(CRGB::Red);
  ledTest(CRGB::Green);
  ledTest(CRGB::Blue);

  // Reset brightness.
  FastLED.setBrightness(brightness);

  // Connect to WIFI.
  Serial.println("Search for wifi...");
  MyWifi* myWifi = new MyWifi(leds);
  myWifi->connect();

  // Connect to time servers.
  connectTime();
}

/**
 * Run the show.
 */
void loop() {

  if (DEBUG) {
    showTime(12, 30, 45);
    delay(10000);
    showTime(3, 30, 45);
    delay(10000);
    showTime(6, 0, 15);
    delay(10000);
    showTime(11, 2, 3);
    delay(10000);
  } else {
   simpleTime();
  }
}
