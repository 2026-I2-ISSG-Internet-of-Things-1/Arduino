#include <Wire.h>
#include <LiquidCrystal_I2C.h>
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

// === LCD I2C ===
LiquidCrystal_I2C lcd(0x27, 20, 4);

// === LED RGB ===
int redPin = 11;
int greenPin = 10;
int bluePin = 9;

// === Buzzer ===
int buzzerPin = 8;
#define NOTE_C4 262

// === Couleurs RGB ===
const int colors[3][3] = {
  {255, 0, 0},
  {0, 255, 0},
  {0, 0, 255}
};
int colorIndex = 0;
unsigned long lastColorChange = 0;
const unsigned long ledInterval = 1000;

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  tone(buzzerPin, NOTE_C4, 500);
  delay(500);
  noTone(buzzerPin);
}

void loop() {
  // --- Température (toujours calculée mais non affichée) ---
  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2 * logR2 + c3 * pow(logR2, 3))) - 273.15;

  // --- Luminosité ---
  lightLevel = analogRead(LightSensorPin);
  lightPercent = (lightLevel / 1023.0) * 100.0;

  Serial.print("Luminosity: ");
  Serial.print(lightLevel);
  Serial.print(" / 1023   ");
  Serial.print(lightPercent, 1);
  Serial.println(" %");

  // --- Affichage LCD ---
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(lightLevel);
  lcd.print(" / 1023");

  lcd.setCursor(0, 1);
  lcd.print("Lum = ");
  lcd.print(lightPercent, 1);
  lcd.print(" %");

  // --- LED RGB ---
  unsigned long currentMillis = millis();
  if (currentMillis - lastColorChange >= ledInterval) {
    setColor(colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2]);
    colorIndex = (colorIndex + 1) % 3;
    lastColorChange = currentMillis;
  }

  delay(500);
}

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}
