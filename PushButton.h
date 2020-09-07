class PushButton
{
 
  int buttonPin;
  int buttonState;
  int lastButtonState;
  unsigned long lastDebounceTime;
  unsigned long debounceDelay = 10;
  int reading;

  public:
  //constructor
  PushButton(int pin)
  {
    buttonPin = pin;
    pinMode(buttonPin, INPUT);
    lastButtonState = LOW;
    lastDebounceTime = 1;
    
  }//constructor

  int WasPressed()
  {
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

    // If the switch changed, due to noise or pressing:
    if (reading != lastButtonState) {
      // reset the debouncing timer
      lastDebounceTime = millis();
    }
  
    if ((millis() - lastDebounceTime) > debounceDelay) {
      // whatever the reading is at, it's been there for longer than the debounce
      // delay, so take it as the actual current state:
      // if the button state has changed:
      if (reading != buttonState) {
        buttonState = reading;
  
        // only do something if button was pushed, not just released
        if (buttonState == HIGH) {
          Serial.print(" Button Pushed ");
          return 1;
        }
        else {
          Serial.print(" Button Released ");
          return 0;
        }
      }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
  }//WasPressed

  
  
};//class
