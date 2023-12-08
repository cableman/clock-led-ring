#include "FastLED.h"
#include "my_wifi.h"

/**
 * Constructor.
 */
MyWifi::MyWifi(CRGB* leds) 
{
  this->leds = leds;
}

/**
 * Connect to wifi network.
 * 
 * Creds should be set in wifi_creds.h.
 */
void MyWifi::connect()
{
  int active = 0;

  // Loop over creds until connection is established.
  while (!this->tryConnect(this->creds.ssid[active], this->creds.password[active], active)) {
    active = ((active + 1) % WifiCredsNumber);
  }
}

/**
 * Try to connect to a network.
 */
bool MyWifi::tryConnect(const char* ssid, const char* password, int active)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  bool state = true;
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts) {    

    leds[active] = CRGB::Red;

    // Show the leds (only one of which is set to white, from above)
    FastLED.show();

    // Wait a little bit
    FastLED.delay(200);

    // Turn our current led back to black for the next loop around
    leds[active] = CRGB::Black;
    FastLED.show();

    FastLED.delay(1000);

    state = !state;
    attempts += 1;
  }
  
  return WiFi.status() == WL_CONNECTED;
}
