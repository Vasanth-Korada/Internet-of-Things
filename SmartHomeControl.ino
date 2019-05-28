#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "YOU";
const char* password = "sbvizag2019";

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
  Serial.begin(115200);
  Serial.println();
  pinMode(D1,OUTPUT);
  pinMode(D2, OUTPUT);
  wifiConnect();
  mqttConnect();
}

void loop() {
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
  if (client.subscribe(topic)) {
    Serial.println("subscribe to cmd OK");
  } else {
    Serial.println("subscribe to cmd FAILED");
  }
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
