#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Thermistor parameters
int ThermistorPin = A0;
int Vo;
float R1 = 10000;
float logR2, R2, T;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

// Initialize I2C LCD at address 0x27, 20 columns and 4 rows
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  Serial.begin(9600);
  lcd.init();            // Initialize the LCD
  lcd.backlight();       // Turn on the backlight
}

void loop() {
  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
  T = T - 273.15; // Convert Kelvin to Celsius
  //T = (T * 9.0) / 5.0 + 32.0; // Uncomment to display in Fahrenheit

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp = ");
  lcd.print(T, 2); // 2 decimal places
  lcd.print(" C");

  delay(500);
}
