#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>

// === Thermistor ===
int ThermistorPin = A0;
int Vo;
float R1 = 10000;
float logR2, R2, T;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

// === LCD I2C ===
LiquidCrystal_I2C lcd(0x27, 20, 4); // Adresse I2C, 20x4

// === Bouton ===
#define BUTTON_PIN 4

// === LED RGB (Cathode commune ici) ===
int redPin = 11;
int greenPin = 10;
int bluePin = 9;

// === Buzzer ===
int buzzerPin = 8;
#define NOTE_C4 262  // Do4

// === Couleurs RGB ===
const int colors[3][3] = {
  {255, 0, 0},   // Rouge
  {0, 255, 0},   // Vert
  {0, 0, 255}    // Bleu
};
int colorIndex = 0;
unsigned long lastColorChange = 0;
const unsigned long ledInterval = 1000; // 1 seconde

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // --- Buzzer initial : Do4 pendant 500ms ---
  tone(buzzerPin, NOTE_C4, 500);
  delay(500);
  noTone(buzzerPin);
}

void loop() {
  // --- Lecture température ---
  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2 * logR2 + c3 * pow(logR2, 3))) - 273.15;

  // --- Lecture bouton ---
  byte buttonState = digitalRead(BUTTON_PIN);
  String buttonMsg = (buttonState == LOW) ? "Button: PRESSED" : "Button: Released";
  Serial.println(buttonMsg);

  // --- Affichage LCD ---
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp = ");
  lcd.print(T, 2);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print(buttonMsg);

  // --- LED RGB (non bloquante) ---
  unsigned long currentMillis = millis();
  if (currentMillis - lastColorChange >= ledInterval) {
    setColor(colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2]);
    colorIndex = (colorIndex + 1) % 3;
    lastColorChange = currentMillis;
  }

  delay(500); // Petit délai général (sans bloquer LED)
}

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}
