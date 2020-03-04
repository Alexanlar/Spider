#ifndef Spider_h
#define Spider_h

#include <ServoEasing.h>

#define DEFAULT_SERVO_SPEED 200

class Leg
{
  public:
	Leg(int sPin, int kPin, int fPin);
	void start(int type = 0, int off_s = 0, int off_k = 0 , int off_f = 0);
	void move_shold(float angle, bool block = true, int speed = DEFAULT_SERVO_SPEED);
	void move_knee(float angle, bool block = true, int speed = DEFAULT_SERVO_SPEED);
	void move_foot(float angle, bool block = true, int speed = DEFAULT_SERVO_SPEED);
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
	bool firstMove = true;
	// VarSpeedServo shold, knee, foot;
	ServoEasing shold, knee, foot;
};

class Spider
{
  public:
    Spider();
    void start();
	void standup();
	void seat();
	void rotate(float angle);
	
    Leg legBL = Leg(0, 1, 2);
	Leg legBR = Leg(4, 5, 6);
	Leg legFR = Leg(8, 9, 10);
	Leg	legFL = Leg(12, 13, 14);
};

int scope_servo(int a);
int scope_joint(int a, int max, int min);
#endif
