#include <math.h>

// === Thermistor ===
int ThermistorPin = A0;
int Vo;
float R1 = 10000;
float logR2, R2, T;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

// === Photoresistance ===
int LightSensorPin = A1;
int lightLevel = 0;
float lightPercent = 0.0;

// === Bouton ===
#define BUTTON_PIN 4

void setup()
{
	Serial.begin(9600);
	pinMode(BUTTON_PIN, INPUT);
}

void loop()
{
	// --- Température ---
	Vo = analogRead(ThermistorPin);
	R2 = R1 * (1023.0 / (float)Vo - 1.0);
	logR2 = log(R2);
	T = (1.0 / (c1 + c2 * logR2 + c3 * pow(logR2, 3))) - 273.15;

	// --- Luminosité ---
	lightLevel = analogRead(LightSensorPin);
	lightPercent = (lightLevel / 1023.0) * 100.0;

	// --- Communication série ---
	if (Serial.available())
	{
		String cmd = Serial.readStringUntil('\n');
		cmd.trim();

		if (cmd == "GET TEMP")
		{
			Serial.print("TEMP:");
			Serial.println(T, 2);
		}
		else if (cmd == "GET LUM")
		{
			Serial.print("LUM:");
			Serial.println(lightPercent, 1);
		}
		else if (cmd == "GET BUTTON")
		{
			byte buttonState = digitalRead(BUTTON_PIN);
			if (buttonState == HIGH)
			{
				Serial.println("Button: PRESSED");
			}
			else
			{
				Serial.println("Button: Released");
			}
		}
		else
		{
			Serial.println("ERROR:UNKNOWN_CMD");
		}
	}

	delay(100);
}
