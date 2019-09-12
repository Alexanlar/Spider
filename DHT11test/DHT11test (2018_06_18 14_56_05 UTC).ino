#include <Wire.h>

#include <LiquidCrystal_I2C.h>

#include <SimpleDHT.h>

// for DHT11, 
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 3
int pinDHT11 = 3;
SimpleDHT11 dht11;

LiquidCrystal_I2C lcd(0x27,16,2);
void setup() {
  Serial.begin(115200);
  
  pinMode(pinDHT11, INPUT);
  digitalWrite(pinDHT11, HIGH);
  
  lcd.init();                     
  lcd.backlight();// Включаем подсветку дисплея
  lcd.print("Temp:");
  lcd.setCursor(8, 0);
  lcd.print("Humi:");
}

void loop() {
  // start working...
  Serial.println("=================================");
  Serial.println("Sample DHT11...");
  
  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); 
    Serial.println(err);delay(1000);
    lcd.setCursor(0, 1);
    lcd.print("error");
    lcd.setCursor(8, 1);
    lcd.print("error");
    return;
  }
  
  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" *C, ");
  lcd.setCursor(0, 1);
  lcd.print(temperature);
  lcd.print("    ");
  Serial.print((int)humidity); Serial.println(" H");
  lcd.setCursor(8, 1);
  lcd.print(humidity);
  lcd.print("    ");
  
  // DHT11 sampling rate is 1HZ.
  delay(1500);
}
