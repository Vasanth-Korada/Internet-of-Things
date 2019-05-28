#include <ESP8266WiFi.h>
// Replace with your network credentials
const char* ssid     = "YOU1";
const char* password = "sbvizag20191";
// Set web server port number to 80
WiFiServer server(80);
// Variable to store the HTTP request
String header;
// Auxiliar variables to store the current output state
String lightState = "off";
String fanState = "off";
String bulbState = "off";
// Assign output variables to GPIO pins
const int light = D2;
const int fan = D3;
const int bulb = D4;

void setup() {
  Serial.begin(9600);
  // Initialize the output variables as outputs
  pinMode(light, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(bulb, OUTPUT);
  // Set outputs to LOW
  digitalWrite(light, LOW);
  digitalWrite(fan, LOW);
  digitalWrite(bulb, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
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
            if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("Light on");
              lightState = "on";
              digitalWrite(light, HIGH);
            } else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("Light off");
              lightState = "off";
              digitalWrite(light, LOW);
            } else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("Fan on");
              fanState = "on";
              digitalWrite(fan, HIGH);
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("Fan off");
              fanState = "off";
              digitalWrite(fan, LOW);
            }
            else if (header.indexOf("GET /3/on") >= 0) {
              Serial.println("Bulb on");
              bulbState = "on";
              digitalWrite(bulb, HIGH);
            } else if (header.indexOf("GET /3/off") >= 0) {
              Serial.println("Bulb off");
              bulbState = "off";
              digitalWrite(bulb, LOW);
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>Smart Home Automation</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 5  
            client.println("<p>LIGHT - State " + lightState + "</p>");
            // If the lightState is off, it displays the ON button       
            if (lightState=="off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">LIGHT ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">LIGHT OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 4  
            client.println("<p>FAN - State " + fanState + "</p>");
            // If the fanState is off, it displays the ON button       
            if (fanState=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">FAN ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">FAN OFF</button></a></p>");
            }
            // Display current state, and ON/OFF buttons for GPIO 3  
            client.println("<p>AC - State " + bulbState + "</p>");
            // If the fanState is off, it displays the ON button       
            if (bulbState=="off") {
              client.println("<p><a href=\"/3/on\"><button class=\"button\">AC ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/3/off\"><button class=\"button button2\">AC OFF</button></a></p>");
            }
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
