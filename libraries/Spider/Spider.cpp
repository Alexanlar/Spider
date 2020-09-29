#define MAX_SHOLD 135
#define MIN_SHOLD 0
#define MAX_KNEE 140
#define MIN_KNEE 0
#define MAX_FOOT 155
#define MIN_FOOT -50

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

int allServoDegree[12];

//Constructor
Leg::Leg(int sPin, int kPin, int fPin) { // @suppress("Class members should be properly initialized")
	sholdPin = sPin;
	kneePin  = kPin;
	footPin  = fPin;
}

//Инициализация ноги со значениями для конкретного положения
void Leg::start(int type, int off_f = 0, int off_k = 0, int off_s = 0) {
	footInd = foot.attach(footPin);
	kneeInd = knee.attach(kneePin);
	sholdInd = shold.attach(sholdPin);
//	sholdInd = shold.attach(sholdPin, s_def_mills_min, s_def_mills_max);
//	kneeInd = knee.attach(kneePin, k_def_mills_min, k_def_mills_max);
//	footInd = foot.attach(footPin, f_def_mills_min, f_def_mills_max);

	if(type == 0)
	type = legType;
	else
	legType = type;

	footNull = footNull_1 + off_f;
	kneeNull = kneeNull_1 + off_k;
	sholdNull = sholdNull_1 + off_s;
	if(type == 2)
	{
		foot.setReverseOperation(true);
		knee.setReverseOperation(true);
		shold.setReverseOperation(true);
	}
	else
	{
	}
//	sholdNull = scope_servo(sholdNull + off_s);
//	kneeNull = scope_servo(kneeNull + off_k);
//	footNull = scope_servo(footNull + off_f);

	allServoDegree[footInd] = footNull + footNull_off;
	allServoDegree[kneeInd] = kneeNull + kneeNull_off;
	allServoDegree[sholdInd] = sholdNull + sholdNull_off;

	foot.setSpeed(50);
	knee.setSpeed(50);
	shold.setSpeed(50);
	foot.write(footNull + footNull_off);
	knee.write(kneeNull + kneeNull_off);
	shold.write(sholdNull + sholdNull_off);
//	shold.startEaseTo(allServoDegree[sholdInd]);
//	knee.startEaseTo(allServoDegree[sholdInd]);
//	foot.startEaseTo(allServoDegree[sholdInd]);

}

//Движение плеча на угол angle
void Leg::move_shold(float angle, int speed) {
	float a;
//	angle = scope_joint(angle, MAX_SHOLD, MIN_SHOLD);
	a = scope_joint(sholdNull + angle, MAX_SHOLD, MIN_SHOLD);
//			shold.easeTo(a);
	allServoDegree[sholdInd] = a;
	sholdFirst = false;
	sholdAngle = a;
}

//Движение колена на угол angle
void Leg::move_knee(float angle, int speed) {
	float a;
//	angle = scope_joint(angle, MAX_KNEE, MIN_KNEE);
	a = scope_joint(kneeNull + kneeNull_off - angle, MAX_KNEE, MIN_KNEE);
//			knee.easeTo(a);
	allServoDegree[kneeInd] = a;
	kneeFirst = false;
	kneeAngle = a;
}

//Движение сутупни на угол angle
void Leg::move_foot(float angle, int speed) {
	float a;
//	angle = scope_joint(angle, MAX_FOOT, MIN_FOOT);
	a = scope_joint(footNull + angle, MAX_FOOT, MIN_FOOT);
//			foot.easeTo(a);
	allServoDegree[footInd] = a;
	footFirst = false;
	footAngle = a;
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
//Проверка на то что дотянется и вычисление максимальных координат
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
	float fAngle = atan2(sinFpoint, sqrt(1-sinFpoint*sinFpoint));
//	float fAngle = asin(sinFpoint);
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

	move_shold(round(sAngle), speed);
	move_knee(round(kAngle), speed);
//	move_foot(round(fAngle)<0?round(fAngle):180 - round(fAngle), speed);
	move_foot(90 - round(fAngle), speed);


//	Serial.print(sServoNextPositionArray[sholdInd]);
//	Serial.print(" ");
//	Serial.print(sServoNextPositionArray[kneeInd]);
//	Serial.print(" ");
//	Serial.println(sServoNextPositionArray[footInd]);
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
	move_point(leg_x + Bx, leg_y + By, leg_z, speed);
}

///////////////////////////////////////////////
//Spider
///////////////////////////////////////////////
Spider::Spider() {
}

void Spider::start() {  
	legBL.start(1, 0, -10, 0);
	legBR.start(2, 0, 0, 0);
	legFR.start(1, 0, -10, 0);
	legFL.start(2, 0, 0, 0);
	this->startMove();
}

void Spider::standup(float h){
	int a = 44;
	legBL.move_point(a, a, -1 * h);
	legBR.move_point(a, a, -1 * h);
	legFL.move_point(a, a, -1 * h);
	legFR.move_point(a, a, -1 * h);
	this->height = h;
	this->startMove();
//	this->march();
}

void Spider::seat(){
	int a = 44;
	legBL.move_point(a, a, -40);
	legBR.move_point(a, a, -40);
	legFL.move_point(a, a, -40);
	legFR.move_point(a, a, -40);
	this->startMove();
}

void Spider::rotate(float angle, int speed){
	int a = 55;
	legBL.rotate(angle, speed);
	legBR.rotate(angle, speed);
	legFL.rotate(angle, speed);
	legFR.rotate(angle, speed);
	this->startMove();
	this->march();
}

void Spider::march()
{
	int h1 = -40;
	int h2 = -1 * this->height;
	int a = 44;
	int b = 40;
	setSpeedForAllServos(200);
	legBL.move_point(b, b, h1);
	legFR.move_point(b, b, h1);
	this->startMove();
	legBL.move_point(a, a, h2);
	legFR.move_point(a, a, h2);
	this->startMove();

	legBR.move_point(b, b, h1);
	legFL.move_point(b, b, h1);
	this->startMove();
	legBR.move_point(a, a, h2);
	legFL.move_point(a, a, h2);
	this->startMove();

	setSpeedForAllServos(50);
}
void Spider::move()
{

}
void Spider::startMove()
{

	setDegreeForAllServos(12, allServoDegree[0],
							  allServoDegree[1],
							  allServoDegree[2],
							  allServoDegree[3],
							  allServoDegree[4],
							  allServoDegree[5],
							  allServoDegree[6],
							  allServoDegree[7],
							  allServoDegree[8],
							  allServoDegree[9],
							  allServoDegree[10],
							  allServoDegree[11]);
//	Serial.println("standup");
//	for(int i = 0; i < 12; i++)
//	{
//		Serial.print(sServoNextPositionArray[i]);
//		Serial.print(" ");
//	}
	Serial.println("");
    setEaseToForAllServos();
	synchronizeAllServosAndStartInterrupt(false);

	do
	{
		// here you can call your own program
		delay(REFRESH_INTERVAL / 1000); // optional 20 ms delay - REFRESH_INTERVAL is in Microseconds
	} while (!updateAllServos());
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
