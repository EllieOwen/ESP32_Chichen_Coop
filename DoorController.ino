
#include <Time.h>
#include <TimeLib.h>
#include "config_private.h"
#include "config.h"
#include "Initialize_Time.h"
#include "PushButton.h"
#include "Door.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>//https://github.com/nhatuan84/esp32-sh1106-oled
//in my experience, some of the inexpensive oled displays on Amazon claiming to have a SSD1306 driver actually have a SH1106

Door door1(CLOSE_PIN, OPEN_PIN, STROKE_MILLIS, 0);
PushButton button1(BUTTON_PIN);
Adafruit_SH1106 display(OLED_SDA, OLED_SCL);

time_t t;//seconds since Jan 1, 1970
int t_today;//seconds since midnight
int t_open;//seconds after midnight
int t_close;//seconds after midnight
char buf1[20];
int openHour = OPEN_HOUR;
int openMinute = OPEN_MINUTE;
int closeHour = CLOSE_HOUR;
int closeMinute = CLOSE_MINUTE;
String state;
int flag_auto_open = 1;//prevent repetative auto opening of the door
int flag_auto_close = 1;//prevent repetative auto closing of the door
int previousState;

void setup() {
  Serial.begin(115200);
  delay(5000);
  Serial.println("Serial begin");
  
  display.begin(SH1106_SWITCHCAPVCC, 0x3C); 
  display.clearDisplay();
  
  Clock myClock(1);
  t = time(NULL) + TIME_SHIFT;
  t_today = t - previousMidnight(t);
  Serial.println("time = ");
  Serial.println(t);
  Serial.println(t_today);
  sprintf(buf1, "%02d:%02d:%02d",  hour(t_today), minute(t_today), second(t_today));
  Serial.println(buf1);
  
  t_open = (openHour * 3600) + (openMinute * 60);
  t_close = (closeHour * 3600) + (closeMinute * 60);
  sprintf(buf1, "Open at %02d:%02d:%02d",  hour(t_open), minute(t_open), second(t_open));
  Serial.println(buf1);
  sprintf(buf1, "Close at %02d:%02d:%02d",  hour(t_close), minute(t_close), second(t_close));
  Serial.println(buf1);

  //check if the door should be open or closed
  if (t_today >= t_open  && t_today < t_close){
    Serial.println("Open the door");
    door1.currentStroke = 0;
    door1.Open();
    state = "opening";
    flag_auto_open = 0;
    previousState = 1;//previously was closed  
  }
  else {
    Serial.println("Close the door");
    door1.currentStroke = 55000;
    door1.Close();
    state = "closing";
    flag_auto_open = 0;
    flag_auto_close = 0;
    previousState = 2;//previously was open   
  }
  

}

void loop() {
  // put your main code here, to run repeatedly:
  int t_previous;
  
  t = time(NULL) + TIME_SHIFT;
  t_today = t - previousMidnight(t);
  if (t_previous >= t_today) {//new day, might need to test the special case of midnight opening/closing
    flag_auto_open = 1;
    flag_auto_close = 1;
  }
  t_previous = t_today;
  
  sprintf(buf1, "%02d:%02d:%02d",  hour(t), minute(t), second(t));
  Serial.print("current time is ");
  Serial.println(buf1);

  
  //oled display stuff

  /* set text size, color, cursor position, 
  set buffer with  Hello world and show off*/
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  
  sprintf(buf1, "%02d:%02d:%02d",  hour(t_today), minute(t_today), second(t_today));
  display.println(buf1);

  display.setTextSize(1);
  display.println("Open Time");
  display.setTextSize(2);
  sprintf(buf1, "%02d:%02d",  hour(t_open), minute(t_open));
  display.println(buf1);
  
  display.setTextSize(1);
  display.println("Close Time");
  display.setTextSize(2);
  sprintf(buf1, "%02d:%02d",  hour(t_close), minute(t_close));
  display.println(buf1);

  display.setCursor(80,32);
  display.setTextSize(4);
  //display.println(50);
  display.println(door1.currentStroke/1000);
  
  
  display.display();
  display.clearDisplay();
  //oled display stuff end

  //check if the door is scheduled to open or close
  //  
  if (t_today >= t_open && flag_auto_open){
    door1.Open();
    flag_auto_open = 0;
    previousState = 1;
  }
  if (t_today >= t_close && flag_auto_close){
    door1.Close();
    flag_auto_close = 0;
    flag_auto_open = 0;//this should already be 0
    previousState = 2;
  }
  //

  //check if the door is at the end of the stroke while opening or closing
  if (door1.EndOfStroke() && door1.flag){
    switch (previousState){
    case 1:
    Serial.print("previous State 1");
      previousState = 2;
    break;
    case 2:
    Serial.print("previous State 2");
      previousState = 1;
    break;
    default:
    break;
    }
    Serial.print(" end of stroke ");
    door1.flag = 0 ;
  }
  //

  //check if button was pressed
  
  if (button1.WasPressed())
  {//pressing the button will toggle through open, stop, close, stop, etc
   //if the door is opening or closing the button will stop the action
   //if the door is stopped the button will start the opposite action of before the stop
   //  
   //delay(1000);
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


  
  Serial.print(".");
  //delay(1000);

}
