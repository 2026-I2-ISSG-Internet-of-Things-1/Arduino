#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --- Thermistor ---
int ThermistorPin = A0;
int Vo;
float R1 = 10000;
float logR2, R2, T;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

// --- LCD I2C ---
LiquidCrystal_I2C lcd(0x27, 20, 4); // Adresse I2C, 20x4

// --- Bouton ---
#define BUTTON_PIN 4

// --- LED RGB ---
int redPin = 11;
int greenPin = 10;
int bluePin = 9;

// --- Couleurs à faire défiler ---
const int numColors = 8;
int colors[numColors][3] = {
  {255, 0, 0},    // Rouge
  {0, 255, 0},    // Vert
  {0, 0, 255},    // Bleu
  {255, 255, 0},  // Jaune
  {0, 255, 255},  // Cyan
  {255, 0, 255},  // Magenta
};

int colorIndex = 0;
unsigned long lastColorChange = 0;
const unsigned long colorInterval = 1000; // en ms

void setup() {
  Serial.begin(9600);

  // LCD
  lcd.init();
  lcd.backlight();

  // Bouton
  pinMode(BUTTON_PIN, INPUT);

  // LED RGB
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  // --- Température ---
  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2)) - 273.15;

  // --- Bouton ---
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

  // --- Couleur LED RGB ---
  if (millis() - lastColorChange >= colorInterval) {
    int r = colors[colorIndex][0];
    int g = colors[colorIndex][1];
    int b = colors[colorIndex][2];
    setColor(r, g, b);

    colorIndex = (colorIndex + 1) % numColors;
    lastColorChange = millis();
  }

  delay(500); // Lecture température + bouton chaque 500ms
}

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, 255 - redValue);   // Anode commune
  analogWrite(greenPin, 255 - greenValue);
  analogWrite(bluePin, 255 - blueValue);
}
