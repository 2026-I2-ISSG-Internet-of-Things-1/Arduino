#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// === LCD I2C ===
LiquidCrystal_I2C lcd(0x27, 16, 2);

// === LED RGB ===
int redPin = 11;
int greenPin = 10;
int bluePin = 9;

// === Buzzer ===
int buzzerPin = 8;
#define NOTE_C4 262

// === Couleurs RGB ===
const int colors[3][3] = {
	{255, 0, 0}, // Rouge
	{0, 255, 0}, // Vert
	{0, 0, 255}  // Bleu
};
int colorIndex = 0;

void setup()
{
	Serial.begin(9600);

	lcd.init();
	lcd.backlight();

	pinMode(redPin, OUTPUT);
	pinMode(greenPin, OUTPUT);
	pinMode(bluePin, OUTPUT);
	pinMode(buzzerPin, OUTPUT);

	// Petit bip de démarrage
	tone(buzzerPin, NOTE_C4, 500);
	delay(500);
	noTone(buzzerPin);
}

void loop()
{
	if (Serial.available())
	{
		String cmd = Serial.readStringUntil('\n');
		cmd.trim();

		if (cmd == "CMD BUZZ")
		{
			Serial.println("ACK BUZZ");
			
			tone(buzzerPin, NOTE_C4, 500);
			delay(500);
			noTone(buzzerPin);
		}
		else if (cmd == "CMD RGB NEXT")
		{
			Serial.println("ACK RGB");
			
			setColor(colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2]);
			colorIndex = (colorIndex + 1) % 3;
		}
		else if (cmd.startsWith("LCD "))
		{
			Serial.println("ACK LCD");
			
			String message = cmd.substring(4);

			// Limite à 32 caractères (2 lignes max pour 20x4)
			message = message.substring(0, 32);

			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print(message);
		}
		else
		{
			Serial.println("ERROR:UNKNOWN_CMD");
		}
	}

	delay(50);
}

void setColor(int redValue, int greenValue, int blueValue)
{
	analogWrite(redPin, redValue);
	analogWrite(greenPin, greenValue);
	analogWrite(bluePin, blueValue);
}
x
