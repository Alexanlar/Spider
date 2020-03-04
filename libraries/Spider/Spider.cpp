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
	if(!sholdFirst)
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
	sholdFirst = false;
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
	if(!kneeFirst)
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
	kneeFirst = false;
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
	if(!0)
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
	footFirst = false;
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
void Leg::move_point(float x,
					 float y,
					 float z,
					 bool block,
					 int speed){
	Serial.println("\n---------move_point");
	Serial.print(x);
	Serial.print(" ");
	Serial.print(y);
	Serial.print(" ");
	Serial.println(z);
	leg_x = x;
	leg_y = y;
	leg_z = z;
	
	Coordinates sPoint, kPoint, fPoint;
	float sAngle = atan2(x, y);
	float a, b, c;
	c = sqrt(x*x  + y*y) - C_LEN;
	b = z;
	a = sqrt(c*c + b*b);
//Проверка на то что дотянется и вычисление максималшьных координат
	if(a > (F_LEN + T_LEN))
	{
		int l = (F_LEN + T_LEN);
		float dx, dy;
		dx = cos(sAngle)/C_LEN;
		dy = sin(sAngle)/C_LEN;
		x = ((x - dx) * l)/a + dx;
		y = ((y - dy) * l)/a + dy;
		z = (z * l)/a;
	}
	c = sqrt(x*x  + y*y) - C_LEN;
	b = z;
	a = sqrt(c*c + b*b);
	float sinFpoint = (b*b + c*c - F_LEN*F_LEN - T_LEN*T_LEN)/(2*F_LEN*T_LEN);
//	float fAngle = atan2(sinFpoint, sqrt(1-sinFpoint));
	float fAngle = asin(sinFpoint);
	float cosBe = (F_LEN + T_LEN*sinFpoint)/a;
    float kAngle = PI/2 - atan2(b,c) - acos(cosBe);

    sAngle = degrees(sAngle);
    kAngle = degrees(kAngle);
    fAngle = degrees(fAngle);

	Serial.print(sAngle);
	Serial.print(" ");
	Serial.print(kAngle);
	Serial.print(" ");
	Serial.println(fAngle);

	move_shold(sAngle, block, speed);
	move_foot(fAngle, block, speed);
	move_knee(kAngle, block, speed);
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


void Leg::rotate(float angle, int speed){
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
	move_point(leg_x + Bx, leg_y + By, leg_z, true, speed);
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

void Spider::rotate(float angle, int speed){
	int d = 30;
	legBL.rotate(angle, speed);
	legBR.rotate(angle, speed);
	legFL.rotate(angle, speed);
	legFR.rotate(angle, speed);
	delay(d);
	legBL.move_point(50, 50, -40, true, speed);
	delay(d);
	legBL.move_point(50, 50, -70, true, speed);
	delay(d);
	legBR.move_point(50, 50, -40, true, speed);
	delay(d);
	legBR.move_point(50, 50, -70, true, speed);
	delay(d);
	legFL.move_point(50, 50, -40, true, speed);
	delay(d);
	legFL.move_point(50, 50, -70, true, speed);
	delay(d);
	legFR.move_point(50, 50, -40, true, speed);
	delay(d);
	legFR.move_point(50, 50, -70, true, speed);
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
