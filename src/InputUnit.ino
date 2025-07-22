#include <Arduino.h>
#include <Wire.h>
#include <math.h>

// === Thermistor ===
#define THERMISTOR_PIN A0
const float R1 = 10000.0;
const float c1 = 1.009249522e-03;
const float c2 = 2.378405444e-04;
const float c3 = 2.019202697e-07;

// === LDR ===
#define LDR_PIN A1

// === Button ===
#define BUTTON_PIN 4

// === I2C slave address ===
#define SLAVE_ADDR 0x08

// Data to send on master request
float lastTemp = 0.0;
int lastLux = 0;
byte lastBtn = 0;

void setup()
{
	Wire.begin(SLAVE_ADDR);	  // Join as I2C slave
	Wire.onRequest(sendData); // Data request handler
	pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop()
{
	// Read sensors
	int vo = analogRead(THERMISTOR_PIN);
	float R2 = R1 * (1023.0 / (float)vo - 1.0);
	float logR2 = log(R2);
	lastTemp = (1.0 / (c1 + c2 * logR2 + c3 * pow(logR2, 3))) - 273.15;
	lastLux = analogRead(LDR_PIN);
	lastBtn = (digitalRead(BUTTON_PIN) == LOW) ? 1 : 0;
	delay(200);
}

// I2C request from master: send latest sensor data
void sendData()
{
	char buf[32];
	int len = snprintf(buf, sizeof(buf), "%.2f,%d,%d", lastTemp, lastLux, lastBtn);
	Wire.write((uint8_t *)buf, len);
}
