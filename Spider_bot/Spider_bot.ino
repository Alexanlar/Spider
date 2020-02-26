//#include <VarSpeedServo.h>


#include <Spider.h>
//#include <Servo.h>

Spider bot;
void setup() {
  Serial.begin(9600);
  bot.start();
  bot.seat();
  bot.standup();
}
int x = 50, y = 50, z;
int Xmax = 90;
int Xmin = 0;
int Ymax = 70;
int Ymin = 70;
int Zmax = 0;
int Zmin = -60;
int Xdir = 0, Ydir = 5, Zdir = 10;
int d = 20;
void loop()
{
  //  bot.standup();
    bot.standup();
  delay(1000);
   bot.seat();
  //  bot.rotate(35);
  //   Serial.print(x);
  //   Serial.print("\t");
  //   Serial.println(y);
  //   delay(100);
}
