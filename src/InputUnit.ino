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

// === I2C ===
#define SLAVE_ADDR 0x08

void setup()
{
	Wire.begin(); // Master
	pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop()
{
	// Read temperature
	int vo = analogRead(THERMISTOR_PIN);
	float R2 = R1 * (1023.0 / (float)vo - 1.0);
	float logR2 = log(R2);
	float tempC = (1.0 / (c1 + c2 * logR2 + c3 * pow(logR2, 3))) - 273.15;

	// Read luminosity
	int luxRaw = analogRead(LDR_PIN);

	// Read button
	byte btn = (digitalRead(BUTTON_PIN) == LOW) ? 1 : 0;

	// Send data
	char buf[32];
	snprintf(buf, sizeof(buf), "%.2f,%d,%d", tempC, luxRaw, btn);
	Wire.beginTransmission(SLAVE_ADDR);
	Wire.write(buf);
	Wire.endTransmission();

	delay(500);
}
