#include <Arduino.h>
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

// === State variables ===
float lastTemp = 0.0;
int lastLux = 0;
byte lastBtn = 0;

void setup()
{
	Serial.begin(9600);
	// Indicate this board is the INPUT unit
	Serial.println("INPUT_READY");
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

	// Send data over Serial
	Serial.print(lastTemp, 2);
	Serial.print(',');
	Serial.print(lastLux);
	Serial.print(',');
	Serial.println(lastBtn);
}
