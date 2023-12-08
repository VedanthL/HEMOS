#include <Arduino.h>
#include <ThingSpeak.h>
#include <WiFi.h>

#define CURRENT_SENSOR_PIN A0 // Analog pin for ACS712 current sensor
#define VOLTAGE_SENSOR_PIN 13 // Analog pin for ZMPT101B voltage sensor

#define CURRENT_SENSOR_SENSITIVITY 0.185 // Sensitivity of ACS712 sensor (adjust based on your sensor's specifications)
#define VOLTAGE_SENSOR_VREF 5.0 // Voltage reference for ADC
#define VOLTAGE_SENSOR_DIVIDER_RATIO 11.0 // Voltage divider ratio for ZMPT101B (adjust based on your circuit)

const char *ssid = "Ved +Nord";
const char *password = "your-password";
const char *thingSpeakApiKey = "WWDVT1MNBBZ7AAQP";

const float costPerUnit = 7.0; // Example cost of electricity per unit in INR

WiFiClient client;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  ThingSpeak.begin(client);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    float current;
    float voltage;

    // Read current using ACS712 sensor
    int rawCurrent = analogRead(CURRENT_SENSOR_PIN);
    current = (rawCurrent - 512) * CURRENT_SENSOR_SENSITIVITY;

    // Read voltage using ZMPT101B sensor
    int rawVoltage = analogRead(VOLTAGE_SENSOR_PIN);
    voltage = (rawVoltage / 1024.0) * VOLTAGE_SENSOR_VREF * VOLTAGE_SENSOR_DIVIDER_RATIO;

    // Update ThingSpeak fields
    ThingSpeak.setField(1, current);
    ThingSpeak.setField(2, voltage);

    // Calculate energy consumption in kilowatt-hours (kWh)
    float energyConsumed = (current * voltage) / 1000.0; // Convert power to kilowatts

    // Calculate cost in Indian Rupees
    float cost = energyConsumed * costPerUnit;

    // Update ThingSpeak field for energy consumption and cost
    ThingSpeak.setField(3, energyConsumed);
    ThingSpeak.setField(4, cost);

    // Send data to ThingSpeak
    int httpCode = ThingSpeak.writeFields(thingSpeakApiKey);

    if (httpCode == 200) {
      Serial.println("Data sent to ThingSpeak successfully!");
    } else {
      Serial.println("Failed to send data to ThingSpeak. HTTP error code: " + String(httpCode));
    }
  } else {
    Serial.println("WiFi not connected. Please check your WiFi credentials.");
  }

  delay(10000); // Adjust the delay based on your monitoring requirements
}
