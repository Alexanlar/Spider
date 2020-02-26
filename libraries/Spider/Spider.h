#ifndef Spider_h
#define Spider_h

//#include <VarSpeedServo.h>
 #include <Servo.h>
class Leg
{
  public:
	Leg(int sPin, int kPin, int fPin);
	void start(int type = 0, int off_s = 0, int off_k = 0 , int off_f = 0);
	void move_shold(float angle);
	void move_knee(float angle);
	void move_foot(float angle);
	void offset_shold(float angle);
	void offset_knee(float angle);
	void offset_foot(float angle);
	void move_point(float x, float y, float z);
	void straight();
	void rotate(float angle);
  // private:
	int sholdPin, kneePin, footPin;
	float sholdAngle, kneeAngle, footAngle;
	float sholdNull, kneeNull, footNull;
	int sholdNull_1 = 0, kneeNull_1 = 130, footNull_1 = 180;
	int sholdNull_2 = 140, kneeNull_2 = 0, footNull_2 = 0;
	float leg_x, leg_y, leg_z;
	int legType = 0;
	// VarSpeedServo shold, knee, foot;
	Servo shold, knee, foot;
};

class Spider
{
  public:
    Spider();
    void start();
	void standup();
	void seat();
	void rotate(float angle);
	
    Leg legBL = Leg(2, 6, 10);
	Leg legBR = Leg(3, 7, 11);
	Leg legFR = Leg(4, 8, 12);
	Leg	legFL = Leg(5, 9, 13);
};

int scope_servo(int a);
int scope_joint(int a, int max, int min);
#endif
