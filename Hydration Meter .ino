#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); 

int sensorPin = A0;
float sensorValue = 0;
void setup()
{
  lcd.init();                      
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
}
void loop()
{
  sensorValue = analogRead(sensorPin);  // read the value from the sensor:
  delay(1000);
  if (sensorValue > 600) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(sensorValue);
    lcd.setCursor(0,1);
    lcd.print("Hydrated!");
  }
  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(sensorValue);
    lcd.setCursor(0,1);
    lcd.print("Drink water!");
  }
}