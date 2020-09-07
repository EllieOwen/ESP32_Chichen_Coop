#include <WiFi.h>
#include <Time.h>
#include <TimeLib.h>
#include "config_private.h"
#include "config.h"

//Connects to Wifi and gets the time from the ntp server

class Clock
{

const char* ssid       = HOME_SSID;
const char* password   = WIFI_PASSWORD;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = GMT_OFFSET;
const int   daylightOffset_sec = 3600;//should know when to offset for day light savings

//time_t t;

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}


public:

// Constructor - creates a Clock
Clock(int x)
{
  SetTimeFromServer();
}

void SetTimeFromServer()
{
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  
}

};//class
