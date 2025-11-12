#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <DHT.h>
#include <bh1750.h>

#define DHTPIN 5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
BH1750 lightMeter;

void setup() {
Serial.begin(9600);
Wire.begin();
dht.begin();
if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
  Serial.println("BH1750 Advanced begin");
} else {
  Serial.println("Error initializing BH1750");
}

}

void loop() {
float humidity = dht.readHumidity();
float temperature = dht.readTemperature();
float lux = lightMeter.readLightLevel();
Serial.print("Humidity: ");
Serial.print(humidity);
Serial.print("Temperature: ");
Serial.print(temperature);
Serial.print("Light:");
Serial.print(lux);
}


