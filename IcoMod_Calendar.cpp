/*
  IcoMod_Calendar.cpp - IcoDesk Module to display the calendar. 
  Created by Donemik Zielinski, February 2, 2023. 
*/

#include "Arduino.h"
#include "IcoMod_Calendar.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "TextUtils.h"
#include "ApiUtils.h"

IcoMod_Calendar::IcoMod_Calendar(Adafruit_ST7735* tft, unsigned int colors[], JsonObject &config)
{
  _tft = tft;
  _colors = colors;

  _refreshInterval = 60000; // config["refreshTime"];
  _icalUrl = config["icalUrl"];
}

void IcoMod_Calendar::draw(Adafruit_ST7735* tft)
{

  tft->fillScreen(_colors[0]);

  TextUtils::printLinesCentered(tft, "Sample event", 20, 2, tft->height() / 6 * 1, 1, _colors[1]);

}

void IcoMod_Calendar::onClick()
{
}

void IcoMod_Calendar::initialize()
{
  _nextRefresh = millis();
}

void IcoMod_Calendar::refresh()
{
  if (millis() >= _nextRefresh)
  {
    _nextRefresh = millis() + _refreshInterval;

    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("Can't fetch me.");
      return;
    }

    String url = String(_icalUrl);
    _ical = ApiUtils::httpGETRequest(url.c_str());

    Serial.println(_ical);

    if (_ical != "")
    {
      Serial.println("Parsing Calendar data failed!");
      return;
    }

    draw(_tft);
  }
}