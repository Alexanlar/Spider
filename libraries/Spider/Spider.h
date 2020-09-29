#ifndef Spider_h
#define Spider_h

#include <ServoEasing.h>

#define DEFAULT_SERVO_SPEED 500

extern int allServoDegree[12];

class Leg
{
  public:
	Leg(int sPin, int kPin, int fPin);
	void start(int type, int off_f = 0, int off_k = 0, int off_s = 0);
	void move_shold(float angle, int speed = DEFAULT_SERVO_SPEED);
	void move_knee(float angle, int speed = DEFAULT_SERVO_SPEED);
	void move_foot(float angle, int speed = DEFAULT_SERVO_SPEED);
	void offset_shold(float angle);
	void offset_knee(float angle);
	void offset_foot(float angle);
	void move_point(float x, float y, float z, int speed = DEFAULT_SERVO_SPEED);
	void straight();
	void rotate(float angle, int speed);
  // private:
	int sholdPin, kneePin, footPin;
	int sholdInd, kneeInd, footInd;
	float sholdAngle, kneeAngle, footAngle;
	float sholdNull, kneeNull, footNull;
	int sholdNull_1 = 0, kneeNull_1 = 0, footNull_1 = 0;
	int sholdNull_off = 90, kneeNull_off = 140, footNull_off = 90;
	float leg_x, leg_y, leg_z;
	int legType = 0;
	bool sholdFirst = true, kneeFirst = true, footFirst = true;
	// VarSpeedServo shold, knee, foot;
	ServoEasing shold, knee, foot;
};

class Spider
{
  public:
    Spider();
    void start();
	void standup(float h);
	void seat();
	void rotate(float angle, int speed);
	void startMove();
	void march();
	void move();
	
    Leg legBL = Leg(0, 1, 2);
	Leg legBR = Leg(4, 5, 6);
	Leg legFR = Leg(8, 9, 10);
	Leg	legFL = Leg(12, 13, 14);

//	int def_mills[4][3][2] = {{{513, 2554}, {110, 530}, {110, 530}},
//							 {{513, 2539}, {110, 530}, {110, 530}},
//							 {{537, 2588}, {110, 530}, {110, 530}},
//							 {{537, 2539}, {110, 530}, {110, 530}}};
	float height;
};

int scope_servo(int a);
int scope_joint(int a, int max, int min);
#endif
