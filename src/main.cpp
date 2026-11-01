#include <Arduino.h>

// put function declarations here:
const int soilPin = A0; // Analog input pin connected to sensor AOUT

void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 Baud
}

void loop() {
  Serial.print("Soil Moisture Value: ");
  Serial.println(getSoilMoisture());
  delay(500); // Wait half a second between readings
}

int getSoilMoisture() {
  return analogRead(soilPin); // Read raw analog value (0 - 1023)
}