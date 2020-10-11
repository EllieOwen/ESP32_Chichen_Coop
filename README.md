# ESP32_Chichen_Coop
So far this code will get the time from the internet when the board is first powered up.  Then it will decide if the door should be initially opened or closed.  Then it will open and close the door once a day as scheduled.

Right now the time must be set in the Arduino IDE, then loaded onto the board.  I'll add the ability to change the times after the board is up and running soon.

The first step is to use the ESP32 to open and close the chicken door.  The ESP32 will keep track of the time and open and close the door on a schedual.  The push button can stop, open, or close the door in between scheduled opening and closings.

The oled displays the current time, scheduled opening and closing times, and the amount the door is opened.

Additional functions include monitoring the temperature and adjust heaters and fans.


