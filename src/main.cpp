#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <DHT.h>
#include <bh1750.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid     = "Orbit IoT";
const char* password = "OrbitRyanRD";
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;
const char* mqtt_topic = "2702221122/environment";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
#define DHTPIN 5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
BH1750 lightMeter;

void reconnect(){
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);

        if (client.connect(clientId.c_str())) {
            Serial.println("connected");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}
void turnWifiOn() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
Serial.begin(9600);
turnWifiOn();
client.setServer(mqtt_server, mqtt_port);
Wire.begin(21, 22);
dht.begin();
if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
  Serial.println("BH1750 Advanced begin");
} else {
  Serial.println("Error initializing BH1750");
}

}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

long now = millis();
if(now - lastMsg > 5000){
lastMsg = now;

float humidity = dht.readHumidity();
float temperature = dht.readTemperature();
float lux = lightMeter.readLightLevel();

StaticJsonDocument<256> doc;
doc["temperature"] = temperature;
doc["humidity"] = humidity;
doc["light"] = lux;

char buffer[256];
size_t n = serializeJson(doc, buffer);
client.publish(mqtt_topic, buffer, n);
client.publish(mqtt_topic, buffer, n);
Serial.print("Published to MQTT: ");  
Serial.println(buffer);                
Serial.print("Humidity: ");
Serial.print(humidity);
Serial.print("Temperature: ");
Serial.print(temperature);
Serial.print("Light:");
Serial.println(lux);
delay(2000);
}
}



