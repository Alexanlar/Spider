#define MAX_SHOLD 135
#define MIN_SHOLD 0
#define MAX_KNEE 130
#define MIN_KNEE 0
#define MAX_FOOT 180
#define MIN_FOOT -180

#define C_LEN 26
#define F_LEN 55
#define T_LEN 82

#define BODY_WIDTH 70
#define BODY_LENGTH 70


#include <Arduino.h>
#include <Coordinates.h>

#include <Spider.h>

#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  500 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

//Constructor
Leg::Leg(int sPin, int kPin, int fPin) { // @suppress("Class members should be properly initialized")
	sholdPin = sPin;
	kneePin  = kPin;
	footPin  = fPin;
}

//Инициализация ноги со значениями для конкретного положения
void Leg::start(int type, int off_s, int off_k, int off_f) {
	shold.attach(sholdPin);
	knee.attach(kneePin);
	foot.attach(footPin);

	if(type == 0)
	type = legType;
	else
	legType = type;

	if(type == 1){
		sholdNull = sholdNull_1;
		kneeNull = kneeNull_1;
		footNull = footNull_1;
	}
	else{
		sholdNull = sholdNull_2;
		kneeNull = kneeNull_2;
		footNull = footNull_2;
	}
	sholdNull = scope_servo(sholdNull + off_s);
	kneeNull = scope_servo(kneeNull + off_k);
	footNull = scope_servo(footNull + off_f);
}

//Движение плеча на угол angle
void Leg::move_shold(float angle, bool block, int speed) {
	float a;
	angle = scope_joint(angle, MAX_SHOLD, MIN_SHOLD);
	if(legType == 1){
		a = sholdNull + angle;
	}
	else{
		a = sholdNull - angle;
	}
	if(!firstMove)
	{
		if(!block)
			shold.startEaseTo(scope_servo(a), speed);
		else
		{
			shold.setSpeed(speed);
			shold.easeTo(scope_servo(a));
		}
	}
	else
		shold.write(scope_servo(a));
	sholdAngle = scope_servo(a);
}

//Движение колена на угол angle
void Leg::move_knee(float angle, bool block, int speed) {
	float a;
	angle = scope_joint(angle, MAX_KNEE, MIN_KNEE);
	if(legType == 1){
		a = kneeNull - angle;
	}
	else{
		a = kneeNull + angle;
	}
	if(!firstMove)
	{
		if(!block)
			knee.startEaseTo(scope_servo(a), speed);
		else
		{
			knee.setSpeed(speed);
			knee.easeTo(scope_servo(a));
		}
	}
	else
		knee.write(scope_servo(a));
	kneeAngle = scope_servo(a);
}

//Движение сутупни на угол angle
void Leg::move_foot(float angle, bool block, int speed) {
	float a;
	angle = scope_joint(angle, MAX_FOOT, MIN_FOOT);
	if(legType == 1){
		a = footNull - angle;
	}
	else{
		a = footNull + angle;
	}
	if(!firstMove)
	{
		if(!block)
			foot.startEaseTo(scope_servo(a), speed);
		else
		{
			foot.setSpeed(speed);
			foot.easeTo(scope_servo(a));
		}
	}
	else
		foot.write(scope_servo(a));
//	Serial.print(scope_servo(a));
//	Serial.print(" ");
	footAngle = scope_servo(a);
}

// void Leg::move(float angle_s, float angle_k, float angle_f, int speed = DEFAULT_SERVO_SPEED){

// }
//TODO: границы при сдвиге
// void Leg::offset_shold(float angle) {
	// float a = shold.read();
	// a = scope_servo(a - angle);
	// shold.write(a);
// }
// void Leg::offset_knee(float angle) {
	// float a = knee.read();
	// a = scope_servo(a - angle);
	// knee.write(a);
// }
// void Leg::offset_foot(float angle) {
	// float a = foot.read();
	// a = scope_servo(a - angle);
	// foot.write(a);
// }

//Перемещение конца ноги в точку с координатами x,y,z в мм Инверсная кинематика
void Leg::move_point(float x, float y, float z){
	leg_x = x;
	leg_y = y;
	leg_z = z;
	
	Coordinates sPoint, kPoint, fPoint;
	
//	float l = sqrt(sq(sqrt(sq(x) + sq(y)) - C_LEN) + sq(z)); //растояние до точки
	float xy = sqrt(sq(x) + sq(y)) - C_LEN; //растояние на плоскости xy

//https://habr.com/ru/post/156579/
//	 Moving to local Coxa-Femur-target coordinate system
//	 Note the case when hDist <= _cFemurOffset. This is for the blind zone.
//	 We never can't reach the point that is nearer to the _cStart then
//	 femur offset (_fStartFarOffset)
//	 float localDestX = hDist <= _cFemurOffset
//		 ? - _fStartFarOffset
//		 : sqrt(sqr(hDist) - sqr(_cFemurOffset)) - _fStartFarOffset;
//
//	 float localDestY = dest.z - _fStartZOffset;
//
//	 Check reachability
//	 float localDistSqr = sqr(localDestX) + sqr(localDestY);
//	 if (localDistSqr > sqr(_fLength + _tLenght))
//	 {
//		 log("Can't reach!");
//		 return false;
//	 }
	
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
	
	move_shold(sAngle);
	move_foot(fAngle);
	move_knee(kAngle);
	while(shold.isMoving() && knee.isMoving() && foot.isMoving()) {
		// Serial.print(shold.isMoving());
		// Serial.print(" ");
		// Serial.print(knee.isMoving());
		// Serial.print(" ");
		// Serial.println(foot.isMoving());
		delay(10);
	}
}

void Leg::straight() {
	move_shold(45);
	move_knee(100);
	move_foot(90);
}


void Leg::rotate(float angle){
	Coordinates pointA, pointB;
	
	pointA.fromCartesian(BODY_WIDTH/2, BODY_LENGTH/2);
//	float r = pointA.getR();
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
	move_point(leg_x + Bx, leg_y + By, leg_z);
}

///////////////////////////////////////////////
//Spider
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
	for(int i = -60; i >= -90; i-=10)
	{
		legBL.move_point(a, a, i);
		legBR.move_point(a, a, i);
		legFL.move_point(a, a, i);
		legFR.move_point(a, a, i);
		legBL.firstMove = false;
		legBR.firstMove = false;
		legFL.firstMove = false;
		legFR.firstMove = false;
		delay(50);
	}
}

void Spider::seat(){
int a = 55;
	for(int i = -60; i <= -40; i+=10)
	{
		legBL.move_point(a, a, i);
		legBR.move_point(a, a, i);
		legFL.move_point(a, a, i);
		legFR.move_point(a, a, i);
		delay(50);
	}
}

void Spider::rotate(float angle){
	int d = 30;
	legBL.rotate(angle);
	legBR.rotate(angle);
	legFL.rotate(angle);
	legFR.rotate(angle);
	delay(d);
	legBL.move_point(50, 50, -40);
	delay(d);
	legBL.move_point(50, 50, -70);
	delay(d);
	legBR.move_point(50, 50, -40);
	delay(d);
	legBR.move_point(50, 50, -70);
	delay(d);
	legFL.move_point(50, 50, -40);
	delay(d);
	legFL.move_point(50, 50, -70);
	delay(d);
	legFR.move_point(50, 50, -40);
	delay(d);
	legFR.move_point(50, 50, -70);
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
