class Door
{
  //
  //Door will be controlled by two pins going to the relay.
  //Door can have 3 states:
  //  0 = Stopped at some point between fully opened and fully closed
  //  1 = Openning
  //  2 = Closing
  //The time for one full extention or retraction is the stroke time in seconds
  //starting with zero seconds at fully closed state

  //Opening and closing the door will not stop other processes from happening
  
  // Class Member Variables
  // These are initialized at startup
  int _extendPin;
  int _retractPin;
  //int _state;
  int _strokeLength = 55000;//in millis, the time it takes to complete a full stroke
  unsigned long _previousTime;
  unsigned long _now;
 
  // These maintain the current state
  // Constructor - creates a Door
  // and initializes the member variables and state
  public:
  int State;
  int currentStroke;//millis, where on the stroke the door is between 0 for closed and the strokeLength for fully opend
  
  Door(int pin1, int pin2, int state)
  {
  _extendPin = pin1;
  _retractPin = pin2;
  State = state;
  pinMode(_extendPin, OUTPUT);  
  pinMode(_retractPin, OUTPUT);   
    
  }

  void Open()
  {
      Serial.println("Opening Door");
      digitalWrite(_extendPin, LOW);
      digitalWrite(_retractPin, HIGH);
      _previousTime = millis();
      State =1;//opening
      //delay(500);
  }
  
  void Close()
  {
      Serial.println("Closing Door");
      digitalWrite(_retractPin, LOW);
      digitalWrite(_extendPin, HIGH);
      _previousTime = millis();
      State = 2;//closing
      //delay(500);
  }

  void Stop()
  {
      Serial.println("STOP");
      digitalWrite(_retractPin, LOW);
      digitalWrite(_extendPin, LOW);
      State = 0;//
  }

  int EndOfStroke()
  {
    switch (State){
      case 1:
        _now = millis();
        currentStroke += _now - _previousTime;
        Serial.print(currentStroke / 1000);
        Serial.print(" ");
        _previousTime = _now;
        if (currentStroke >= _strokeLength){
          currentStroke = _strokeLength - 1;
          return 1;
        }
        else {
          return 0;
        }
      break;
      case 2:
        _now = millis();
        currentStroke -= millis() - _previousTime;
        Serial.print(currentStroke / 1000);
        Serial.print(" ");
        _previousTime = _now;
        if (currentStroke <= 0){
          currentStroke = 1;
          return 1;
        }
        else {
          return 0;
        }
      break;
      default:
      //not moving
      Serial.print(".");
      return 1;
      break;
    }//switch
    
  }
  
};
