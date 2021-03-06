#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#define DHTPIN 14
#define DHTTYPE DHT11 

const char* ssid = "Provakar_Wifi-Network";
const char* password = "linkingparkcool";
const char* mqtt_server = "iot.eclipse.org";

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  pinMode(D4, OUTPUT);
  pinMode(D2, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  dht.begin();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if (payload[0] == '1'){
    Serial.println("Glow LED inbuit");
    digitalWrite(D4, LOW);   
  }
  else if (payload[0] == '2'){
    Serial.println("Inbuilt LED Off");
    digitalWrite(D4, HIGH);   
  }
  else if (payload[0] == '3'){
    Serial.println("LED 1 ON");
    digitalWrite(D2, HIGH);
  }
  else if (payload[0] == '4'){
    Serial.println("LED 1 Off");
    digitalWrite(D2, LOW);
  }

  

}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client-baka")) {
      Serial.println("connected");
      client.subscribe("baka/room1/lamp");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

//  if(t>30.0){
//    client.publish("baka/room1/triggerAC", "1");
//  }
  
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  if (isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  else{
      Serial.print("Temperature = ");
      Serial.println(temperature);
  }

  Serial.print("Humidity = ");
  Serial.println(humidity);

  client.publish("baka/room1/temp", String(temperature).c_str());
  client.publish("baka/room1/humid", String(humidity).c_str());
  delay(2000);
}
