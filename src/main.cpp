#include <Arduino.h>
#include <SPI.h>
#include <WiFi101.h>
#include "net.h"

char ssid[] = SSID;
char pass[] = PASSWORD;

// Server settings
const char server[] = IP;   // or an IP like "192.168.1.50"
const int  port     = PORT;
const char path[]   = "/api/moisture";

const int soilPin = A0;

const unsigned long SEND_INTERVAL = 15UL * 60UL * 1000UL; // 15 minutes in ms
unsigned long lastSend = 0;
bool firstSend = true;

// Function declarations (needed in a .cpp file)
int  getSoilMoisture();
void connectWiFi();
bool sendReading(int value);

void setup() {
  Serial.begin(9600);
  // Only wait briefly for Serial, so the board still runs when not plugged into a PC
  unsigned long t = millis();
  while (!Serial && millis() - t < 3000);

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present!");
    while (true);
  }
  connectWiFi();
}

void loop() {
  unsigned long now = millis();

  if (firstSend || now - lastSend >= SEND_INTERVAL) {
    firstSend = false;
    lastSend = now;

    connectWiFi();                  // does nothing if already connected
    int value = getSoilMoisture();
    Serial.print("Soil Moisture Value: ");
    Serial.println(value);
    sendReading(value);
  }
}

int getSoilMoisture() {
  return analogRead(soilPin);
}

void connectWiFi() {
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);
    delay(10000);
  }
  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());
}

bool sendReading(int value) {
  WiFiClient client;

  if (!client.connect(server, port)) {
    Serial.println("Connection to server failed");
    return false;
  }

  String body = "{\"moisture\":" + String(value) + "}";

  client.print("POST ");
  client.print(path);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(server);
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(body.length());
  client.println("Connection: close");
  client.println();
  client.print(body);

  // Print the server's response (5 s timeout)
  unsigned long timeout = millis();
  while (client.connected() && millis() - timeout < 5000) {
    while (client.available()) {
      Serial.write(client.read());
      timeout = millis();
    }
  }
  client.stop();
  return true;
}