/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com
*********/
// Load Wi-Fi library
#define LOGGING
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <PubSubClient.h>

#define DEVICE_NAME "ESP8266_og"
#define NET_SSID "SSIDeh"
#define NET_PW "password"

#define MQTT_DOMAIN "192.168.1.38"
#define MQTT_PORT 1883

#define TOPIC "house/room/office"
#define MEASUREMENT "climate"

const int oneWireBus = 4; // pin D2

WiFiClient espClient;
PubSubClient client(espClient);

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

void reconnectToNetwork() {
  Serial.print("(Re)Connecting to ");
  Serial.println(NET_SSID);
  WiFi.begin(NET_SSID, NET_PW);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(9600);
  reconnectToNetwork();
  sensors.begin();
}

float readTemperature() {
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  return temperatureC;
}

String line_protocol() {
  return String(MEASUREMENT) + " temperature=" + String(readTemperature());
}

void publish() {
  client.setServer(MQTT_DOMAIN, MQTT_PORT);
  if(!client.connect("ESP8266Client")) {
    Serial.println(client.state());
    return;
  }

  Serial.println("connected");
  Serial.println(line_protocol());
  client.publish(TOPIC, line_protocol().c_str());
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    reconnectToNetwork();
  }
  publish();
  // delay(300000); // 5s in ms
  delay(3000);
}
