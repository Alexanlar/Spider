#ifndef Spider_h
#define Spider_h

#include <ServoEasing.h>

#define DEFAULT_SERVO_SPEED 500

extern int allServoDegree[12];

class Leg
{
  public:
	Leg(int sPin, int kPin, int fPin, int ind);
	void start(int type, int off_f = 0, int off_k = 0, int off_s = 0);
	void move_shold(float angle, int speed = DEFAULT_SERVO_SPEED);
	void move_knee(float angle, int speed = DEFAULT_SERVO_SPEED);
	void move_foot(float angle, int speed = DEFAULT_SERVO_SPEED);
	void offset_shold(float angle);
	void offset_knee(float angle);
	void offset_foot(float angle);
	void move_point(float x, float y, float z);
	void straight();
	void rotateZ(float angle);
	void rotateX(float angle);
	void rotateXY(float angle);
  // private:
	int sholdPin, kneePin, footPin;
	int sholdInd, kneeInd, footInd;
	float sholdAngle, kneeAngle, footAngle;
	float sholdNull, kneeNull, footNull;
	int sholdNull_1 = 0, kneeNull_1 = 0, footNull_1 = 0;
	int sholdNull_off = 45, kneeNull_off = 140, footNull_off = 155;
	float leg_x, leg_y, leg_z;
	int legType = 0;
	int ind = 0;
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
	void rotate(float angle);
	void startMove();
	void march();
	void move(int distX, int distY);
	void move2(int dist);
	void step(int legInd, float dist);
	void step2(int legInd, float distX, float distY);
	void bodyMove(float x, float y, float z);
	Leg * getLegByInd(int ind);
	
    Leg legBL = Leg(0, 1, 2, 1);
	Leg legBR = Leg(4, 5, 6, 2);
	Leg legFR = Leg(8, 9, 10, 3);
	Leg	legFL = Leg(12, 13, 14, 4);

	static const int dhStep = 40;
	static const int dhStep2 = 10;
//	int def_mills[4][3][2] = {{{513, 2554}, {110, 530}, {110, 530}},
//							 {{513, 2539}, {110, 530}, {110, 530}},
//							 {{537, 2588}, {110, 530}, {110, 530}},
//							 {{537, 2539}, {110, 530}, {110, 530}}};
	float height;
	float body_x, body_y, body_z;
};

int scope_servo(int a);
int scope_joint(int a, int max, int min);
#endif
