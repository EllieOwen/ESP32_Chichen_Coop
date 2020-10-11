
//for NodeMCU ESP32S
#define OLED_SDA 21
#define OLED_SCL 22
#define OPEN_PIN 4
#define CLOSE_PIN 15
#define BUTTON_PIN 2

//open and closing time based on 24 hour clock
#define OPEN_HOUR 8
#define OPEN_MINUTE 0
#define CLOSE_HOUR 20
#define CLOSE_MINUTE 30

#define GMT_OFFSET -18000//seconds
#define DAYLIGHT_SAVINGS_OFFSET 3600//seconds
#define TIME_SHIFT -14400 //seconds from GMT//Eastern Daylight Savings adjustment

#define STROKE_MILLIS 55000//millis to fully extend or retract actuator
