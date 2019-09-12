#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  lcd.noBacklight();// Включаем подсветку дисплея 
  delay(1000);
  lcd.backlight();// Включаем подсветку дисплея
}
