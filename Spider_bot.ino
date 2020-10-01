#include <Spider.h>
#include <PlotPlus.h>
#define MIN_PULSE_WIDTH 650
#define MAX_PULSE_WIDTH 2350
#define DEFAULT_PULSE_WIDTH 1500
#define FREQUENCY 60

Spider spider;
ServoEasing shold, knee, foot;
int angle, spd = 150;
unsigned long time, dTime;
void setup ()
{
	Serial.begin(9600);
	Serial.println("Start programm Spider_bot");
	spider.start();
	setSpeedForAllServos(spd);
	setEasingTypeForAllServos(EASE_CUBIC_OUT);
    setEaseToForAllServos();
	spider.standup(60);
	time = millis();
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
			   setSpeedForAllServos(serialReadInt());
			   break;
			case 2:
				spider.standup(serialReadInt());
				break;
			case 3:
			{
				int a = serialReadInt();
				spider.legFL.rotateXY(a);
				spider.legFR.rotateXY(a);
				spider.legBL.rotateXY(-a);
				spider.legBR.rotateXY(-a);
				spider.startMove();
				break;
			}
			case 4:
			{
				Serial.println("Input coordinates:...");
				while (Serial.available() == false)delay(50);
				int x=serialReadInt() ,y=serialReadInt(), z=serialReadInt();
				Serial.println(x);
				Serial.println(y);
				Serial.println(z);
				spider.bodyMove(x, y, z);
			    break;
			}
			case 5:
			{
				Serial.println("legBL Input coordinates:...");
				while (Serial.available() == false)delay(50);
				int x=serialReadInt() ,y=serialReadInt(), z=serialReadInt();
				Serial.println(x);
				Serial.println(y);
				Serial.println(z);
				spider.legBL.move_point(x, y, z);
				spider.startMove();
			    break;
			}
			case 6:
			{
				Serial.println("legFR Input coordinates:...");
				while (Serial.available() == false)delay(50);
				int x=serialReadInt() ,y=serialReadInt(), z=serialReadInt();
				Serial.println(x);
				Serial.println(y);
				Serial.println(z);
				spider.legFR.move_point(x, y, z);
				spider.startMove();
			    break;
			}
			case 7:
			{
				Serial.println("legBR Input coordinates:...");
				while (Serial.available() == false)delay(50);
				int x=serialReadInt() ,y=serialReadInt(), z=serialReadInt();
				Serial.println(x);
				Serial.println(y);
				Serial.println(z);
				spider.legBR.move_point(x, y, z);
				spider.startMove();
			    break;
			}
			case 8:
			{
				Serial.println("Move:...");
				spider.move(serialReadInt(), serialReadInt());;
//				spider.standup(80);
				break;
			}
			default:
			{
				Serial.print("Move foot microS: ");
				Serial.println(IntVar);
				spider.bodyMove(20, 20, 50);
				spider.standup(spider.height);

			}
		}
		time = millis();
//		for(int i = 0; i < 12; i++)
//		{
//			Serial.print(allServoDegree[i]);
//			Serial.print(" ");
//		}
//		Serial.println("");

    }

	dTime = millis() - time;
	if(dTime > 15000 && spider.height != 20)
	{
		spider.standup(20);
	}

	test_bot();
//	spd += 10;
//	setSpeedForAllServos(spd);

}

void test_bot()
{
	spider.standup(70);
	spider.move(0, 50);
	spider.move(0, 50);
	spider.move(0, 50);
	spider.standup(spider.height);
	spider.rotate(30);
	spider.rotate(30);
	spider.rotate(30);
	spider.rotate(30);
	spider.bodyMove(20, 20, spider.height);
	delay(500);
	spider.bodyMove(-40, 0, spider.height);
	delay(500);
	spider.bodyMove(0, -40, spider.height);
	delay(500);
	spider.bodyMove(40, 0, spider.height);
	delay(500);
	spider.standup(spider.height);
	delay(500);
	spider.standup(35);
	delay(500);
	spider.standup(70);
	delay(500);
	spider.standup(100);
//	spider.move(50);
	time = millis();
	delay(1000);
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
