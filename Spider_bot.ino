#include <Spider.h>
#include <PlotPlus.h>
#define MIN_PULSE_WIDTH 650 
#define MAX_PULSE_WIDTH 2350 
#define DEFAULT_PULSE_WIDTH 1500
#define FREQUENCY 60

Spider spider;
int angle, spd = 150;
void setup ()
{
  spider.start();
  Serial.begin(9600);
  spider.standup();
}
void loop ()
{
	Serial.println("\n---------move_point");
//	   delay(500);
// spider.seat();
//   delay(500);

    spider.rotate(20, 50);
    delay(2000);
//    spider.legBR.move_point(0, 80, -82, false, 50);
//    delay(500);
//    spider.legBR.move_point(0, 80, 500, true, 500);
//    delay(2000);
//    spider.legBR.move_point(50, 0, 0);
//    delay(2000);
//    int x,y,z;
//    bool a = true;
//    for(x = 0; x < 200; x+=10)
//    {
//     for(y = a?0:200; a?y < 200:y > 0; a?y+=10:y-=10)
//     {
//       for(z = -100; z < 0; z+=10)
//       {
//	plotWith(Serial, x, y, z);
//        Serial.print(x);
//        Serial.print(" ");
//        Serial.print(y);
//        Serial.print(" ");
//        Serial.print(z);
//        Serial.print("\n");
//         spider.legBR.move_point(x, 0, -60);
//       }
//     }
//     a = !a;
//    }
//    spider.legBR.shold.setSpeed(spd);
//    spider.legBR.shold.easeTo(135);
//   delay(500);
}
