#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --- Paramètres du capteur de température ---
int ThermistorPin = A0;
int Vo;
float R1 = 10000;
float logR2, R2, T;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

// --- Écran LCD I2C ---
LiquidCrystal_I2C lcd(0x27, 20, 4); // Adresse 0x27, 20 colonnes, 4 lignes

// --- Bouton ---
#define BUTTON_PIN 4

void setup() {
  Serial.begin(9600);

  // Initialisation LCD
  lcd.init();
  lcd.backlight();

  // Initialisation du bouton
  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  // Lecture de la température
  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
  T = T - 273.15;

  // Lecture de l'état du bouton
  byte buttonState = digitalRead(BUTTON_PIN);
  String buttonMsg;

  if (buttonState == LOW) {
    buttonMsg = "Button: PRESSED";
    Serial.println("Button is pressed");
  } else {
    buttonMsg = "Button: Released";
    Serial.println("Button is not pressed");
  }

  // Affichage sur l'écran LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp = ");
  lcd.print(T, 2);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print(buttonMsg);

  delay(500);
}
