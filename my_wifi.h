#include <ESP8266WiFi.h>
#include "wifi_creds.h"

#include <FastLED.h>

#ifndef __my_wifi_h
#define __my_wifi_h

#define maxAttempts 15

class MyWifi
{
  private:
    CRGB* leds;
    WifiCreds creds;
    bool tryConnect(const char* ssid, const char* password, int active);
  
  public:
    // Constructor(s)
    MyWifi(CRGB* leds);

    void connect();
};

#endif
