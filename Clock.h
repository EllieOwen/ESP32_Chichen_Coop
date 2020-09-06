#include <WiFi.h>
#include <Time.h>
#include <TimeLib.h>

//Connects to Wifi and gets the time from the ntp server

class Clock
{

const char* ssid       = "yourssid";
const char* password   = "yourpassword";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -5*3600;
const int   daylightOffset_sec = 3600;

time_t t;

void TestPrintAll(time_t t)
{
  Serial.println(day(t));
  Serial.println(hour(t));//looks like GMT
  Serial.println(minute(t));
  Serial.println(second(t));
  Serial.println(weekday(t));
  Serial.println(month(t));
  Serial.println(year(t));
  Serial.println(monthStr(month(t)));
}

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

void PrintLocalTime()
{
  Serial.print( "-.");
  printLocalTime();
  
}



};//class
