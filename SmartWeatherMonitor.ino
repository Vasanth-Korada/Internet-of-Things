 #include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
//-------- Customise these values -----------
const char* ssid = "YOU";
const char* password = "sbvizag2019";
//String command;
#include "DHT.h"
#define DHTPIN D3    // what pin we're connected to
#define DHTTYPE DHT11   // define type of sensor DHT 11
DHT dht (DHTPIN, DHTTYPE);
  
#define ORG "tx63nr"
#define DEVICE_TYPE "VK_1"
#define DEVICE_ID "vk0614"
#define TOKEN "c9ZeDJPys0vJTGQ)nv"
//-------- Customise the above values --------
 
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/Data/fmt/json";
char topic1[] = "iot-2/cmd/home/fmt/String";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
String command;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
 
WiFiClient wifiClient;
void callback(char* topic1, byte* payload, unsigned int payloadLength);
PubSubClient client(server, 1883, callback, wifiClient);

void setup() {
 Serial.begin(115200);
 Serial.println();
 pinMode(D1,OUTPUT);
 pinMode(D2, OUTPUT);
 dht.begin();
 Serial.print("Connecting to ");
 Serial.print(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 } 
 Serial.println("");
 wifiConnect();
 mqttConnect();
 
 Serial.print("WiFi connected, IP address: ");
 Serial.println(WiFi.localIP());
}
 
void loop() {
float h = dht.readHumidity();
float t = dht.readTemperature();
if (!client.loop()) {
    mqttConnect();
  }
if (isnan(h) || isnan(t))
{
Serial.println("Failed to read from DHT sensor!");
delay(1000);
return;
}
PublishData(t,h);
 if (!client.loop()) {
    mqttConnect();
  }
delay(100);
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
void wifiConnect() {
  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("nWiFi connected, IP address: "); Serial.println(WiFi.localIP());
}
void callback(char* topic, byte* payload, unsigned int payloadLength) {
  Serial.print("callback invoked for topic: "); Serial.println(topic);

  for (int i = 0; i < payloadLength; i++) {
    //Serial.println((char)payload[i]);
    command += (char)payload[i];
  }
Serial.println(command);
if(command == "LIGHTON"){
  digitalWrite(D1,HIGH);
  Serial.println("Light is Switched ON");
}
else if(command == "LIGHTOFF"){
  digitalWrite(D1,LOW);
  Serial.println("Light is Switched OFF");
}
else if(command == "FANON"){
  digitalWrite(D2,HIGH);
  Serial.println("FAN is Switched ON"); 
}
else if(command == "FANOFF"){
  digitalWrite(D2,LOW);
  Serial.println("FAN is Switched OFF");
}



command ="";
}
void PublishData(float temp, float humid){
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }
  String payload = "{\"d\":{\"temperature\":";
  payload += temp;
  payload+="," "\"humidity\":";
  payload += humid;
  payload += "}}";
 Serial.print("Sending payload: ");
 Serial.println(payload);
  
 if (client.publish(topic, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}
