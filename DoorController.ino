#include <WiFi.h>
#include <Time.h>
#include <TimeLib.h>
#include "config.h"
#include "Door.h"
#include "PushButton.h"
#include "Clock.h"
//#include "SimpleWiFiServer.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>//https://github.com/nhatuan84/esp32-sh1106-oled

//Door will be controlled by one button.
//Door will be controlled by automatic clock
//Door will be controlled by WiFi on LAN
//
////for NodeMCU ESP32S
//#define OLED_SDA 21
//#define OLED_SCL 22
//#define OPEN_PIN 4
//#define CLOSE_PIN 15
//#define BUTTON_PIN 2

//#define TIME_SHIFT -14400 //seconds from GMT//Eastern Daylight Savings adjustment


Clock myClock(1);
//SimpleWiFiServer mySimpleWiFiServer(1);

Door door1(CLOSE_PIN, OPEN_PIN, 0);
PushButton button1(BUTTON_PIN);
Adafruit_SH1106 display(OLED_SDA, OLED_SCL);

int previousState = 1;//assumes the door started off fully closed
time_t t;
int t_today;//seconds since midnight today
int t_open;//seconds after midnight to open door
int t_close;//seconds after midnight to close door
time_t openDoor;
time_t closeDoor;
int openHour = 8;
int openMinute = 0;
int closeHour = 21;
int closeMinute = 0;
int autoOpen;
int autoClose;
int wiFiServerReturn;
String myTime;
char buf1[20];
String response;
int doorMoving = 1;

//WiFiServer server(80);

void setup()
{
  Serial.begin(115200);
  display.begin(SH1106_SWITCHCAPVCC, 0x3C); 
  display.clearDisplay();
  //myClock.SetTimeFromServer();
  //Serial.print("Response is ");
  //Serial.println(response);
  t = time(NULL) + TIME_SHIFT;//seconds since Jan 1, 1970 local time
  t_today = t - previousMidnight(t);//seconds since midnight
  Serial.print("time =   ");
  Serial.print(t);
  Serial.print("  seconds since midnight = ");
  Serial.println(t_today);
  t_open = (openHour * 3600) + (openMinute * 60);
  t_close = (closeHour * 3600) + (closeMinute * 60);
  openDoor = (openHour * 3600) + (openMinute * 60);
  closeDoor = (closeHour * 3600) + (closeMinute * 60);
  //openDoor = t + 10;
  //closeDoor = openDoor + 60;
  Serial.print("open at  ");
  Serial.println(openDoor);
  Serial.println(t_open);  
  Serial.print("close at ");
  Serial.println(closeDoor);
  Serial.println(t_close);
  
  if (t_today >= openDoor){
    autoOpen = 0;
    autoClose = 1;
  }//if
  else {
    autoOpen = 1;
    autoClose = 0;
    
  }//else

  if (t_today >= closeDoor){
    autoOpen = 1;
    autoClose = 0;
  }//if

//  mySimpleWiFiServer.Begin();
    
}//setup

void loop()
{

//  wiFiServerReturn = mySimpleWiFiServer.ListenForClients();
//  switch (wiFiServerReturn){
//    case 1:
//      door1.Open();
//    break;
//    case 2:
//      door1.Close();
//    break;
//    case 0:
//    door1.Stop();
//    break;
//    default:
//    break;
//  }
  
  //t = time(NULL) + TIME_SHIFT;
  t = time(NULL) + TIME_SHIFT;
  t_today = t - previousMidnight(t);
  
  sprintf(buf1, "%02d:%02d:%02d",  hour(t), minute(t), second(t));
  Serial.print("current time is ");
  Serial.println(buf1);
  
  //this code assumes the door will open once in the morning and close once in the evening
  if (autoOpen == 1 && t_today >= t_open)
  {
    autoOpen = 0;
    autoClose = 1;
    //openDoor += 86400;
    //openDoor += 120;
    door1.Open();
    Serial.print(" Auto Opened at "); 
     myClock.PrintLocalTime();
  }
  else {
   // Serial.println("not autoOpen");
  }
  
  if (autoClose == 1 && t_today >= t_close)
  {
    autoOpen = 1;
    autoClose = 0;
    //closeDoor += 86400;
    //closeDoor += 120;
    door1.Close();
    Serial.print(" Auto Closed at ");
    myClock.PrintLocalTime();
  }
  else{
   // Serial.println("not autoClose");
  }

  
  //myClock.PrintLocalTime();
  if (door1.EndOfStroke()){
    switch (previousState){
    case 1:
      previousState = 2;
    break;
    case 2:
      previousState = 1;
    break;
    default:
    break;
    }
    door1.Stop();
  }
//  else {
//    doorMoving = 1;
//    display.setTextSize(2);
//    display.setTextColor(WHITE);
//    display.setCursor(0,0);
//    display.clearDisplay();
//    sprintf(buf1, "%02d:%02d:%02d",  hour(t), minute(t), second(t));
//    display.println(buf1);
//    display.setTextSize(4);
//    display.println(door1.currentStroke/1000);
//    display.display();
//    display.clearDisplay();
//    
//  }
  
  if (button1.WasPressed())
  {//pressing the button will toggle through open, stop, close, stop, etc
   //if the door is opening or closing the button will stop the action
   //if the door is stopped the button will start the opposite action of before the stop
  
    switch (door1.State) {
    case 1:
    Serial.println("case 1");
      previousState = 1;
      door1.Stop();
      break;
    case 2:
    Serial.println("case 2");
      previousState = 2;
      door1.Stop();
      break;
    case 0:
    Serial.print("case 0");
    Serial.print( " previous State ");
    Serial.println(previousState);
      if (previousState == 2){
        Serial.println("reopen door");
        door1.Open();
      }
      else if (previousState == 1){
        Serial.println("reoclose door");
        door1.Close();
      }
      else {
        Serial.print("Something went wrong ");
      }
      break;
    default:
      door1.Stop();
      break;
    }//switch
  }//if


  //display stuff

  /* set text size, color, cursor position, 
  set buffer with  Hello world and show off*/
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  
  //display.println("Current Time");
  //display.println(t_today);
  

  sprintf(buf1, "%02d:%02d:%02d",  hour(t_today), minute(t_today), second(t_today));
  display.println(buf1);
  //display.display();

  display.setTextSize(1);
  display.println("Open Time");
  display.setTextSize(2);
  sprintf(buf1, "%02d:%02d",  hour(openDoor), minute(openDoor));
  display.println(buf1);
  
  display.setTextSize(1);
  display.println("Close Time");
  display.setTextSize(2);
  sprintf(buf1, "%02d:%02d",  hour(closeDoor), minute(closeDoor));
  display.println(buf1);

  display.setCursor(80,32);
  display.setTextSize(4);
  display.println(door1.currentStroke/1000);
  
  
  display.display();
  display.clearDisplay();
  
//  // send data only when you receive data:
//  if (Serial.available() > 0) {
//    // read the incoming byte:
//   // incomingByte = Serial.read();
//   a = Serial.readString();
//
//    // say what you got:
//    Serial.print("I received: ");
//    Serial.println(a);
//    display.clearDisplay();
//    display.println(a);
//    display.display();
//    delay(2000);
//    display.clearDisplay();
//  }

  
  delay(10);//this delay is to slow down the looping
}
