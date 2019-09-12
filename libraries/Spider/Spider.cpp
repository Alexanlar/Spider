#define MAX_SHOLD 135
#define MIN_SHOLD 0
#define MAX_KNEE 130
#define MIN_KNEE 0
#define MAX_FOOT 180
#define MIN_FOOT -180

#define PI 3,14

#define C_LEN 26
#define F_LEN 55
#define T_LEN 82

#define BODY_WIDTH 70
#define BODY_LENGTH 70

#include <Arduino.h>
#include <Coordinates.h>
#include <VarSpeedServo.h>
// #include "Servo.h"
#include <Spider.h>

Leg::Leg(int sPin, int kPin, int fPin) {
	this->sholdPin = sPin;
	this->kneePin  = kPin;
	this->footPin  = fPin;
}

void Leg::start(int type = 0, int off_s = 0, int off_k = 0 , int off_f = 0) {
	this->shold.attach(this->sholdPin);
	this->knee.attach(this->kneePin);
	this->foot.attach(this->footPin);

	if(type == 0)
	type = legType;
	else
	legType = type;

	if(type == 1){
		this->sholdNull = this->sholdNull_1;
		this->kneeNull = this->kneeNull_1;
		this->footNull = this->footNull_1;
	}
	else{
		this->sholdNull = this->sholdNull_2;
		this->kneeNull = this->kneeNull_2;
		this->footNull = this->footNull_2;
	}
	this->sholdNull = scope_servo(this->sholdNull + off_s);
	this->kneeNull = scope_servo(this->kneeNull + off_k);
	this->footNull = scope_servo(this->footNull + off_f);
	// this->shold.write(this->sholdNull);
	// this->foot.write(this->footNull);
	// this->knee.write(this->kneeNull);
	
	// Serial.println("sholdNull");
	// Serial.println(sholdNull);
	// Serial.println("kneeNull");
	// Serial.println(kneeNull);
	// Serial.println("footNull");
	// Serial.println(footNull);
	// Serial.println(" ");
}

void Leg::move_shold(float angle) {
	float a;
	angle = scope_joint(angle, MAX_SHOLD, MIN_SHOLD);
	if(legType == 1){
		a = this->sholdNull + angle;
	}
	else{
		a = this->sholdNull - angle;
	}
	this->shold.write(scope_servo(a));
	this->sholdAngle = scope_servo(a);
}
void Leg::move_knee(float angle) {
	float a;
	angle = scope_joint(angle, MAX_KNEE, MIN_KNEE);
	if(legType == 1){
		a = this->kneeNull - angle;
	}
	else{
		a = this->kneeNull + angle;
	}
	this->knee.write(scope_servo(a));
	this->kneeAngle = scope_servo(a);
}
void Leg::move_foot(float angle) {
	float a;
	angle = scope_joint(angle, MAX_FOOT, MIN_FOOT);
	if(legType == 1){
		a = this->footNull - angle;
	}
	else{
		a = this->footNull + angle;
	}
	this->foot.write(scope_servo(a));
	this->footAngle = scope_servo(a);
}

//TODO: границы при сдвиге
void Leg::offset_shold(float angle) {
	float a = shold.read();
	a = scope_servo(a - angle);
	shold.write(a);
}
void Leg::offset_knee(float angle) {
	float a = knee.read();
	a = scope_servo(a - angle);
	knee.write(a);
}
void Leg::offset_foot(float angle) {
	float a = foot.read();
	a = scope_servo(a - angle);
	foot.write(a);
}

//Перемещение конца ноги в точку с координатами x,y,z в мм
void Leg::move_point(float x, float y, float z){
	leg_x = x;
	leg_y = y;
	leg_z = z;
	
	Coordinates sPoint, kPoint, fPoint;
	
	float l = sqrt(sq(sqrt(sq(x) + sq(y)) - C_LEN) + sq(z));
	float xy = sqrt(sq(x) + sq(y)) - C_LEN;
	
	// Find joint as circle intersect ( equations from http://e-maxx.ru/algo/circles_intersection & http://e-maxx.ru/algo/circle_line_intersection )

	float A = -2 * xy;
	float B = -2 * z;
	float C = sq(xy) + sq(z) + sq(F_LEN) - sq(T_LEN);
	float X0 = -A * C / (sq(A) + sq(B));
	float Y0 = -B * C / (sq(A) + sq(B));
	float D = sqrt( sq(F_LEN) - (sq(C) / (sq(A) + sq(B))) );
	float mult = sqrt( sq(D) / (sq(A) + sq(B)));
	float ax, ay, bx, by;
	ax = X0 + B * mult;
	bx = X0 - B * mult;
	ay = Y0 - A * mult;
	by = Y0 + A * mult;
	// Select solution on top as joint
	float jointLocalL = ((ax > bx) ? ax : bx);
	float jointLocalZ = (ax > bx) ? ay : by;
	
	sPoint.fromCartesian(x, y);
	kPoint.fromCartesian(jointLocalL, jointLocalZ);
	fPoint.fromCartesian(xy - jointLocalL, z - jointLocalZ);
	// Serial.println("in class");
	// Serial.println("sPoint");
	// Serial.println(sPoint.getAngle() * 180/PI);
	// Serial.println("kPoint");
	// Serial.println(kPoint.getAngle() * 180/PI);
	// Serial.println("fPoint");
	// Serial.println(fPoint.getAngle() * 180/PI);
	// Serial.println("xy");
	// Serial.println(xy);
	// Serial.println("jointLocalL");
	// Serial.println(jointLocalL);
	// Serial.println("jointLocalZ");
	// Serial.println(jointLocalZ);
	// Serial.println(" ");
	float sAngle = 90 - (sPoint.getAngle() * 180/PI);
	sAngle = scope_joint(sAngle, MAX_SHOLD, MIN_SHOLD);

	float kAngle = (kPoint.getAngle() * 180/PI);
	if(kAngle > 180)
		kAngle = kAngle  - 360;
	kAngle = 90 - kAngle;
	kAngle = scope_joint(kAngle, MAX_KNEE, MIN_KNEE);
	
	float fAngle = fPoint.getAngle() * 180/PI;
	if(fAngle > 180)
		fAngle = fAngle  - 360;
	fAngle = fAngle + kAngle;
	fAngle = scope_joint(fAngle, MAX_FOOT, MIN_FOOT);
	
	this->move_shold(sAngle);
	this->move_foot(fAngle);
	this->move_knee(kAngle);
}

void Leg::straight() {
	this->move_shold(45);
	this->move_knee(100);
	this->move_foot(90);
}


void Leg::rotate(float angle){
	Coordinates pointA, pointB;
	
	pointA.fromCartesian(BODY_WIDTH/2, BODY_LENGTH/2);
	float r = pointA.getR();
	float newAngle;
	if(legType == 1){
		newAngle = scope_joint(90 - (pointA.getAngle() * 180/PI) + angle, MAX_SHOLD, MIN_SHOLD);
	}
	else{
		newAngle = scope_joint(90 - (pointA.getAngle() * 180/PI) - angle, MAX_SHOLD, MIN_SHOLD);
	}
	pointB.fromPolar(pointA.getR(), (90 - newAngle) * PI/180);
	float Bx = pointB.getX() - BODY_WIDTH/2; 
	float By = pointB.getY() - BODY_LENGTH/2;
	
	Serial.print(leg_x);
	Serial.print("\t");
	Serial.print(leg_y);
	Serial.print("\t");
	Serial.print(leg_z);
	Serial.print("\t");
	Serial.print(Bx);
	Serial.print("\t");
	Serial.println(By);
	this->move_point(leg_x + Bx, leg_y + By, leg_z);
}

///////////////////////////////////////////////
Spider::Spider() {
}

void Spider::start() {  
	legBL.start(1, 0, 0, -20);
	legBR.start(2, -10, 5, 30);
	legFR.start(1, 0, -5, -30);
	legFL.start(2, 0, 0, 25);
}

void Spider::standup(){	
int a = 55;
	for(int i = -60; i >= -70; i--)
	{
		this->legBL.move_point(a, a, i);
		this->legBR.move_point(a, a, i);
		this->legFL.move_point(a, a, i);
		this->legFR.move_point(a, a, i);
		delay(50);
	}
}

void Spider::seat(){
int a = 55;
	for(int i = -60; i <= -40; i++)
	{
		this->legBL.move_point(a, a, i);
		this->legBR.move_point(a, a, i);
		this->legFL.move_point(a, a, i);
		this->legFR.move_point(a, a, i);
		delay(50);
	}
}

void Spider::rotate(float angle){
	int d = 30;
	this->legBL.rotate(angle);
	this->legBR.rotate(angle);
	this->legFL.rotate(angle);
	this->legFR.rotate(angle);
	delay(d);
	this->legBL.move_point(50, 50, -40);
	delay(d);
	this->legBL.move_point(50, 50, -70);
	delay(d);
	this->legBR.move_point(50, 50, -40);
	delay(d);
	this->legBR.move_point(50, 50, -70);
	delay(d);
	this->legFL.move_point(50, 50, -40);
	delay(d);
	this->legFL.move_point(50, 50, -70);
	delay(d);
	this->legFR.move_point(50, 50, -40);
	delay(d);
	this->legFR.move_point(50, 50, -70);
}

////////////////////////////////////////
int scope_servo(int a){
	if(a < 0)
		return 0;
	if(a > 180)
		return 180;
	else
		return a;
}

int scope_joint(int a, int max, int min){
	if(a < min)
		return min;
	if(a > max)
		return max;
	else
		return a;
}