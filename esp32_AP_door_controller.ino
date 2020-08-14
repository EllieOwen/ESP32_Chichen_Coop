/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  

  Modified by Leon Schmidt
  Chicken Coop Door controller
*********/

// Load Wi-Fi library
#include <WiFi.h>

// Replace with your network credentials
const char* ssid     = "ESP32-AP";
const char* password = "1234";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;
//elapse time variables so the process doesn't stop by using the delay function
unsigned long startTime;
unsigned long now;
unsigned long maxCycle = 5000;

// Auxiliar variables to store the current output state
String output4State = "off";
String output5State = "off";

// Assign output variables to GPIO pins
const int output4 = 4;
const int output5 = 5;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output4, OUTPUT);
  pinMode(output5, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output4, LOW);
  digitalWrite(output5, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /open") >= 0) {
              Serial.println("GPIO 5 off");
              output5State = "off";
              digitalWrite(output5, LOW);
              Serial.println("GPIO 4 on");
              output4State = "on";
              digitalWrite(output4, HIGH);
              startTime = millis();
            } else if (header.indexOf("GET /close") >= 0) {
              Serial.println("GPIO 4 off");
              output4State = "off";
              digitalWrite(output4, LOW);
              Serial.println("GPIO 5 on");
              output5State = "on";
              digitalWrite(output5, HIGH);
              startTime = millis();
            } else if (header.indexOf("GET /stop") >= 0) {
              Serial.println("STOP");
              Serial.println("GPIO 4 off");
              Serial.println("GPIO 5 off");
              output4State = "off";
              output5State = "off";
              digitalWrite(output4, LOW);
              digitalWrite(output5, LOW);
            }

            if (millis() - startTime >= maxCycle) {//need to account for maximum number roll over
              Serial.println("GPIO 4 off");
              Serial.println("GPIO 5 off");
              output4State = "off";
              output5State = "off";
              digitalWrite(output4, LOW);
              digitalWrite(output5, LOW);
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: red;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
            client.println("<body><h2>Chicken Coop Controller</h2>");
            
            // Display current state, and OPEN/CLOSE buttons for GPIO 4 and 5  
            client.println("<p>GPIO 4 - State " + output4State + "</p>");  
            client.println("<p>GPIO 5 - State " + output5State + "</p>");
            // If the output4State is off, it displays the OPEN button       
            if (output4State=="off") {
              client.println("<p><a href=\"/open\"><button class=\"button\">OPEN</button></a></p>");
            } else {
              client.println("<p><a href=\"/close\"><button class=\"button button2\">CLOSE</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 27  
            //client.println("<p>GPIO 27 - State " + output27State + "</p>");
            // If the output27State is off, it displays the ON button       
            //if (output27State=="off") {
              client.println("<p><a href=\"/stop\"><button class=\"button\">STOP</button></a></p>");
            //} else {
            //  client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
            //}
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
