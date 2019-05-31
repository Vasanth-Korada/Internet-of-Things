#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "YOU";
const char* password = "sbvizag2019";

#define OLED_RESET D5
/* Object named display, of the class Adafruit_SSD1306 */
Adafruit_SSD1306 display(OLED_RESET);

char buf[50];
int ledPin = 12; // choose pin for the LED
int inputPin = 13; // choose input pin (for Infrared sensor) 
int val = 0; // variable for reading the pin status
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
#define ORG "tx63nr"
#define DEVICE_TYPE "VK_1"
#define DEVICE_ID "vk0614"
#define TOKEN "c9ZeDJPys0vJTGQ)nv"

String command;
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/cmd/home/fmt/String";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
//Serial.println(clientID);

WiFiClient wifiClient;
void callback(char* topic, byte* payload, unsigned int payloadLength);
PubSubClient client(server, 1883, callback, wifiClient);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); /* Initialize display with address 0x3C */
  Serial.begin(115200);
  Serial.println();
  wifiConnect();
  mqttConnect();
  display.clearDisplay();  /* Clear display */
  display.setTextSize(1.5);  /* Select font size of text. Increases with size of argument. */
  display.setTextColor(WHITE);  /* Color of text*/
  pinMode(ledPin, OUTPUT); // declare LED as output 
  pinMode(inputPin, INPUT); // declare Infrared sensor as input
  testdrawcircle();
  
}

void loop() {
  if (!client.loop()) {
    mqttConnect();
  }
  val = digitalRead(inputPin); // read input value 
   if (val == LOW )
   { // check if the input is HIGH
      digitalWrite(ledPin, LOW); // turn LED OFF   
   } 
   else 
   { 
      digitalWrite(ledPin, HIGH); // turn LED ON 
   } 
  delay(100);
}

void wifiConnect() {
  Serial.print("Connecting to "); 
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("nWiFi connected, IP address: "); Serial.println(WiFi.localIP());
}

void mqttConnect() {
  if (!client.connected()) {
    Serial.print("Reconnecting MQTT client to "); Serial.println(server);
    while (!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      delay(500);
    }
    initManagedDevice();
    Serial.println();
  }
}

void initManagedDevice() {
  if (client.subscribe(topic)) {
    Serial.println("subscribe to cmd OK");
  } else {
    Serial.println("subscribe to cmd FAILED");
  }
}

void callback(char* topic, byte* payload, unsigned int payloadLength) {
  Serial.print("callback invoked for topic: "); 
  Serial.println(topic);

  for (int i = 0; i < payloadLength; i++) {
    //Serial.println((char)payload[i]);
    command += (char)payload[i];
  }
  Serial.println(command);
  display.clearDisplay();
  command.toCharArray(buf, payloadLength+1);
  char old_cmd[20];
  old_cmd == buf;

  
  if(command == "ADS ON"){
    drawStr(25,25,old_cmd);
    Serial.println("ADSON");
  }
  if(command == "ADS OFF"){
  command = "";
  command.toCharArray(buf, payloadLength+1);
  drawStr(25,25,buf);
  Serial.println("ADS OFF");
}

  val = digitalRead(inputPin);
  if(val == HIGH)
  {
    command = "";
    command.toCharArray(buf, payloadLength+1);
    drawStr(25,25,"HI :)");
    if(val == LOW){
    command.toCharArray(buf, payloadLength+1);
    drawStr(25,25,buf);
  }
  }
  
drawStr(25,25,buf);
display.display();

command ="";
}

void drawStr(uint8_t x, uint8_t y, char* str){
  display.setCursor(x, y);  /* Set x,y coordinates */
  display.println(str);
}
void testdrawcircle(void) {
  display.clearDisplay();

  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}
