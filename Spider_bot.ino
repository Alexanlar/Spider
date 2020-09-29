#include <Spider.h>
#include <PlotPlus.h>
#define MIN_PULSE_WIDTH 650
#define MAX_PULSE_WIDTH 2350
#define DEFAULT_PULSE_WIDTH 1500
#define FREQUENCY 60

Spider spider;
ServoEasing shold, knee, foot;
int angle, spd = 100;
void setup ()
{
	Serial.begin(9600);
	Serial.println("Start programm Spider_bot");
	spider.start();
	setSpeedForAllServos(spd);
	setEasingTypeForAllServos(EASE_QUADRATIC_OUT);
    setEaseToForAllServos();
	spider.standup(40);
//	Serial.print("0deg = ");
//	Serial.println(spider.legFR.foot.DegreeToMicrosecondsOrUnits(0));
//	Serial.print("180deg = ");
//	Serial.println(spider.legFR.foot.DegreeToMicrosecondsOrUnits(180));
//	Serial.print("0deg = ");
//	Serial.println(spider.legFL.foot.DegreeToMicrosecondsOrUnits(0));
//	Serial.print("180deg = ");
//	Serial.println(spider.legFL.foot.DegreeToMicrosecondsOrUnits(180));
}
void loop ()
{
	int IntVar = 0;
    if (Serial.available() > 0)
    {  //если есть доступные данные
//		for(int i = 0; i < 12; i++)
//		{
//			Serial.print(allServoDegree[i]);
//			Serial.print(" ");
//		}
//		Serial.println("");
    	delay(200);
    	IntVar = serialReadInt();
		Serial.println(IntVar);
		switch(IntVar)
		{
			case 1:
				spider.march();
			   break;
			case 2:
				spider.standup(serialReadInt());
				break;
			case 3:
				spider.rotate(serialReadInt(), spd);
				break;
			case 4:
			{
				Serial.println("Input coordinates:...");
				while (Serial.available() == false)delay(50);
				int x=serialReadInt() ,y=serialReadInt(), z=serialReadInt();
				Serial.println(x);
				Serial.println(y);
				Serial.println(z);
				spider.legFR.move_point(x, y, z);
				spider.startMove();
			    break;
			}
			case 5:
			{
				Serial.println("Input foot angle:...");
				int a = serialReadInt();
				spider.legFR.foot.easeTo(a);
				spider.legFL.foot.easeTo(a);
				spider.legBR.foot.easeTo(a);
				spider.legBL.foot.easeTo(a);
				Serial.print("spider.legFR.getDeltaMicrosecondsOrUnits = ");
				Serial.println(spider.legFR.foot.getDeltaMicrosecondsOrUnits());
				break;
			}
			case 6:
			{
				Serial.println("Input knee angle:...");
				int a = serialReadInt();
				spider.legFR.knee.easeTo(a);
				spider.legFL.knee.easeTo(a);
				spider.legBR.knee.easeTo(a);
				spider.legBL.knee.easeTo(a);
				Serial.print("spider.legFR.kneeAngle = ");
				Serial.println(spider.legFR.kneeAngle);
				break;
			}
			case 7:
			{
				Serial.println("Input should angle:...");
				int a = serialReadInt();
				spider.legFR.shold.easeTo(a);
				spider.legFL.shold.easeTo(a);
				spider.legBR.shold.easeTo(a);
				spider.legBL.shold.easeTo(a);
				Serial.print("spider.legFR.sholdAngle = ");
				Serial.println(spider.legFR.sholdAngle);
				break;
			}
			default:
			{
				Serial.print("Move foot microS: ");
				Serial.println(IntVar);
				spider.legBR.foot.writeMicrosecondsOrUnits(IntVar);
//				spider.legFR.knee.easeTo(IntVar);
//				spider.legFR.shold.easeTo(IntVar);
//				Serial.print("spider.legFR.footAngle = ");
//				Serial.println(spider.legFR.footAngle);
//				Serial.print("spider.legFR.kneeAngle = ");
//				Serial.println(spider.legFR.kneeAngle);
//				Serial.print("spider.legFR.sholdAngle = ");
				//				Serial.println(spider.legFR.kneeAngle);
			}
		}
//		for(int i = 0; i < 12; i++)
//		{
//			Serial.print(allServoDegree[i]);
//			Serial.print(" ");
//		}
//		Serial.println("");
    }

//	   spider.seat();
//	       delay(500);
//	   spider.standup();
//	       delay(500);
//	delay(1000);
}

int serialReadInt()
{
	Serial.print("input int - :");
	while (Serial.available() == false)delay(50);
	int a=0;
	while (Serial.available())
	{
		char c = Serial.read();
		if(c == '-')
			a = (a * 10 + (Serial.read() - '0')) * -1;
		else
			a = a * 10 + (c - '0');
	}
	Serial.print("read - :");
	Serial.println(a);
	return a;
}
