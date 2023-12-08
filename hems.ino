#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADS1015.h>

#define CURRENT_SENSOR_PIN A0 // Analog pin for ACS712 current sensor
#define VOLTAGE_SENSOR_PIN A1 // Analog pin for ZMPT101B voltage sensor

#define CURRENT_SENSOR_SENSITIVITY 0.185 // Sensitivity of ACS712 sensor (adjust based on your sensor's specifications)
#define VOLTAGE_SENSOR_VREF 5.0 // Voltage reference for ADC
#define VOLTAGE_SENSOR_DIVIDER_RATIO 11.0 // Voltage divider ratio for ZMPT101B (adjust based on your circuit)

Adafruit_ADS1115 ads; // Create an ADC object

void setup() {
  Serial.begin(115200);
  Serial.println("Electricity Usage Monitoring System");

  ads.begin();
}

void loop() {
  float current;
  float voltage;

  // Read current using ACS712 sensor
  int rawCurrent = analogRead(CURRENT_SENSOR_PIN);
  current = (rawCurrent - 512) * CURRENT_SENSOR_SENSITIVITY;

  // Read voltage using ZMPT101B sensor
  int rawVoltage = analogRead(VOLTAGE_SENSOR_PIN);
  voltage = (rawVoltage / 1024.0) * VOLTAGE_SENSOR_VREF * VOLTAGE_SENSOR_DIVIDER_RATIO;

  // Print the readings
  Serial.print("Current: ");
  Serial.print(current);
  Serial.println(" A");

  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.println(" V");

  // Calculate power (P = VI)
  float power = current * voltage;
  Serial.print("Power: ");
  Serial.print(power);
  Serial.println(" W");

  delay(1000); // Adjust the delay based on your monitoring requirements
}
