#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

// === I2C ===
#define SLAVE_ADDR 0x08

// === LCD I2C ===
LiquidCrystal_I2C lcd(0x27, 20, 4);

// === LED RGB (Cathode commune) ===
int redPin = 11;
int greenPin = 10;
int bluePin = 9;

// === Buzzer ===
int buzzerPin = 8;
#define NOTE_C4 262 // Do4

// === Couleurs RGB ===
const int colors[3][3] = {
	{255, 0, 0}, // Rouge
	{0, 255, 0}, // Vert
	{0, 0, 255}	 // Bleu
};
int colorIndex = 0;
unsigned long lastColorChange = 0;
const unsigned long ledInterval = 1000;

// === Received data ===
float lastTemp = 0.0;
int lastLux = 0;
int lastBtn = 0;

void receiveEvent(int howMany)
{
	char buf[32];
	int i = 0;
	while (Wire.available() && i < 31)
	{
		buf[i++] = Wire.read();
	}
	buf[i] = '\0';
	sscanf(buf, "%f,%d,%d", &lastTemp, &lastLux, &lastBtn);
}

void setup()
{
	Serial.begin(9600);
	Wire.begin(SLAVE_ADDR);
	Wire.onReceive(receiveEvent);

	lcd.init();
	lcd.backlight();

	pinMode(redPin, OUTPUT);
	pinMode(greenPin, OUTPUT);
	pinMode(bluePin, OUTPUT);
	pinMode(buzzerPin, OUTPUT);

	// Buzzer initial: Do4 pendant 500ms
	tone(buzzerPin, NOTE_C4, 500);
	delay(500);
	noTone(buzzerPin);
}

void loop()
{
	// Display
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Temp = ");
	lcd.print(lastTemp, 2);
	lcd.print(" C");

	lcd.setCursor(0, 1);
	lcd.print("Button: ");
	lcd.print(lastBtn == 1 ? "PRESSED" : "Released");

	lcd.setCursor(0, 2);
	lcd.print("Lux = ");
	lcd.print(lastLux);

	// LED RGB cycling (non-blocking)
	unsigned long currentMillis = millis();
	if (currentMillis - lastColorChange >= ledInterval)
	{
		setColor(colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2]);
		colorIndex = (colorIndex + 1) % 3;
		lastColorChange = currentMillis;
	}

	// Buzzer on button press
	if (lastBtn == 1)
	{
		tone(buzzerPin, 440, 200);
	}
}

void setColor(int redValue, int greenValue, int blueValue)
{
	analogWrite(redPin, redValue);
	analogWrite(greenPin, greenValue);
	analogWrite(bluePin, blueValue);
}
