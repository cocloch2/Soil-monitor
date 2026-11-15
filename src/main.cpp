#include <Arduino.h>
#include <SPI.h>
#include <WiFi101.h>

char ssid[] = "NETGEAR39-5G"; // your network SSID (name)
char pass[] = "wittystar144"; // your network password
int status = WL_IDLE_STATUS; // the WiFi radio's status

// put function declarations here:
const int soilPin = A0; // Analog input pin connected to sensor AOUT

void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 Baud
  while (!Serial); // Wait for Serial to be ready

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present!");
    while (true);
  }
  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  Serial.print("Soil Moisture Value: ");
  Serial.println(getSoilMoisture());
  delay(500); // Wait half a second between readings
}

int getSoilMoisture() {
  return analogRead(soilPin); // Read raw analog value (0 - 1023)
}