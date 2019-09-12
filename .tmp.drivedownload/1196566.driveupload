#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SimpleDHT.h>
int ForwardPin = 5;
int BackwardPin = 3;
int ClosePin = 6;
int OpenPin = 7;
int pinDHT11 = 2;
int s = 160;
bool forward = true;

int upTemp   = 25;
int downTemp = 15;

void open_wind(int spd)
{
  Serial.write("open");
  while(digitalRead(OpenPin) == HIGH)
  {
    analogWrite(ForwardPin, spd);
    digitalWrite(BackwardPin, LOW);
  }
  digitalWrite(ForwardPin, LOW);
  digitalWrite(BackwardPin, LOW);
}

void close_wind(int spd)
{
  while(digitalRead(ClosePin) == HIGH)
  {
    analogWrite(BackwardPin, spd);
    digitalWrite(ForwardPin, LOW);
  }
  digitalWrite(ForwardPin, LOW);
  digitalWrite(BackwardPin, LOW);
}

LiquidCrystal_I2C lcd(0x27,16,2);
SimpleDHT11 dht11;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ForwardPin, OUTPUT);
  pinMode(BackwardPin, OUTPUT);
  pinMode(ClosePin, INPUT);
  pinMode(OpenPin, INPUT);
  pinMode(pinDHT11, INPUT);

  digitalWrite(ClosePin, HIGH);
  digitalWrite(OpenPin, HIGH);
  digitalWrite(pinDHT11, HIGH);

  lcd.init();      
  lcd.noBacklight();// Включаем подсветку дисплея 
  delay(1000);
  lcd.backlight();// Включаем подсветку дисплея
  lcd.print("Temp:");
  lcd.setCursor(8, 0);
  lcd.print("Humi:");
}

void loop() {
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); 
    Serial.println(err);
    lcd.setCursor(0, 1);
    lcd.print("error");
    lcd.setCursor(8, 1);
    lcd.print("error");
    delay(1000);
    return;
  }
  
  Serial.print((int)temperature); Serial.print(" *C, ");
  lcd.setCursor(0, 1);
  lcd.print(temperature);
  lcd.print("    ");
  Serial.print((int)humidity); Serial.println(" H");
  lcd.setCursor(8, 1);
  lcd.print(humidity);
  lcd.print("    ");

  if(temperature >= upTemp)
  {
    open_wind(s);
  }
  if(temperature <= downTemp)
  {
    close_wind(s);
  }
  delay(1500);
}
