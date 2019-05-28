#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#define DHTPIN D3     // what pin we're connected to
#define DHTTYPE DHT11   // define type of sensor DHT 11

DHT dht (DHTPIN, DHTTYPE);

// CHANGE TO YOUR WIFI CREDENTIALS
const char* ssid = "YOU";
const char* password = "sbvizag2019";


// CHANGE TO YOUR DEVICE CREDENTIALS AS PER IN IBM BLUMIX

#define ORG "tx63nr"
#define DEVICE_TYPE "VK_1"
#define DEVICE_ID "vk0614"
#define TOKEN "c9ZeDJPys0vJTGQ)nv"  //  Authentication Token OF THE DEVICE
String command;
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/cmd/home/fmt/String";
char topic1[] = "iot-2/evt/Data/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
//Serial.println(clientID);
void callback(char* topic, byte* payload, unsigned int payloadLength);
WiFiClient wifiClient;
PubSubClient client(server, 1883, callback, wifiClient);

void setup() {
  Serial.println("DHT11 test!");
  dht.begin();
  Serial.begin(115200);
  Serial.println();
  pinMode(D1,OUTPUT);
  wifiConnect();
  mqttConnect();
}

void loop() {
  float h = dht.readHumidity();
float t = dht.readTemperature();
/*if (isnan(h) || isnan(t))
{
Serial.println("Failed to read from DHT sensor!");
return;
}*/
Serial.print("Humidity: ");
Serial.print(h);
Serial.print(" %\t");
Serial.print("Temperature: ");
Serial.print(t);
Serial.println(" *C ");
Serial.print("temp");
delay(2000);
  PublishData(t,h);
  delay(1000);
  if (!client.loop()) {
    mqttConnect();
  }
delay(100);
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
  if (client.subscribe(topic1)) {
    Serial.println("subscribe to cmd OK");
  } else {
    Serial.println("subscribe to cmd FAILED");
  }
}

void callback(char* topic1, byte* payload, unsigned int payloadLength) {
  Serial.print("callback invoked for topic: "); Serial.println(topic);

  for (int i = 0; i < payloadLength; i++) {
    //Serial.println((char)payload[i]);
    command += (char)payload[i];
  }
Serial.println(command);
if(command == "LIGHTON"){
  digitalWrite(D1,LOW);
  Serial.println("Light is Switched ON");
}
else if(command == "LIGHTOFF"){
  digitalWrite(D1,HIGH);
  Serial.println("Light is Switched OFF");
}
command ="";
}



void PublishData(float temp,float humid){
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
  
 if (client.publish(topic1, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}
