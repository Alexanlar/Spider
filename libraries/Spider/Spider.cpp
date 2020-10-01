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

#define DEFAULT_LEG_X 55
#define DEFAULT_LEG_Y 30
#define DEFAULT_LEG_Z 60

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
Leg::Leg(int sPin, int kPin, int fPin, int ind) { // @suppress("Class members should be properly initialized")
	sholdPin = sPin;
	kneePin  = kPin;
	footPin  = fPin;
	this->ind = ind;
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
					 float z){
//	Serial.println("\n---------move_point");
//	Serial.print(x);
//	Serial.print(" ");
//	Serial.print(y);
//	Serial.print(" ");
//	Serial.println(z);
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

//	Serial.print(sAngle);
//	Serial.print(" ");
//	Serial.print(kAngle);
//	Serial.print(" ");
//	Serial.println(fAngle);

	move_shold(round(sAngle));
	move_knee(round(kAngle));
	move_foot(90 - round(fAngle));
}

void Leg::straight() {
	move_shold(45);
	move_knee(100);
	move_foot(90);
}


void Leg::rotateZ(float angle){
	Coordinates pointA, pointB;
	
	pointA.fromCartesian(leg_x + (BODY_WIDTH/2), leg_y + (BODY_LENGTH/2));
	float newAngle;
	if(legType == 1){
		newAngle = scope_joint((pointA.getAngle() * 180/PI) - angle, MAX_SHOLD, MIN_SHOLD);
	}
	else{
		newAngle = scope_joint((pointA.getAngle() * 180/PI) + angle, MAX_SHOLD, MIN_SHOLD);
	}
	pointB.fromPolar(pointA.getR(), (newAngle) * PI/180);
	float Bx = pointB.getX() - BODY_WIDTH/2; 
	float By = pointB.getY() - BODY_LENGTH/2;

	Serial.print("pointB XY: ");
	Serial.print(pointB.getX());
	Serial.print(" ");
	Serial.println(pointB.getY());
	
	Serial.print(leg_x);
	Serial.print("\t");
	Serial.print(leg_y);
	Serial.print("\t");
	Serial.print(leg_z);
	Serial.print("\t");
	Serial.print(Bx);
	Serial.print("\t");
	Serial.println(By);
	move_point(Bx, By, leg_z);
}

void Leg::rotateX(float angle){
	Coordinates pointA, pointB;

	pointA.fromCartesian(leg_y + (BODY_LENGTH/2), leg_z);
	float newAngle;
	Serial.print("Angle : ");
	Serial.println((pointA.getAngle() * 180/PI));
	Serial.print("r : ");
	Serial.println(pointA.getR());
		newAngle = (pointA.getAngle() * 180/PI) - angle;
	Serial.print("newAngle : ");
	Serial.println(newAngle);
	pointB.fromPolar(pointA.getR(), (newAngle) * PI/180);
	float By = pointB.getX() - BODY_LENGTH/2;
	float Bz = pointB.getY();

	Serial.print("pointB XY: ");
	Serial.print(pointB.getX());
	Serial.print(" ");
	Serial.println(pointB.getY());

	Serial.print("pointB XY: ");
	Serial.print(pointB.getX());
	Serial.print(" ");
	Serial.println(pointB.getY());

	Serial.print(leg_x);
	Serial.print("\t");
	Serial.print(leg_y);
	Serial.print("\t");
	Serial.print(leg_z);
	Serial.print("\t By: ");
	Serial.print(By);
	Serial.print("\t Bz: ");
	Serial.println(Bz);
	move_point(leg_x, By, Bz);
}

void Leg::rotateXY(float angle){
	Coordinates pointA, pointB;

	float xy_len = sqrt((leg_y + BODY_LENGTH/2)*(leg_y + BODY_LENGTH/2) + (leg_x + BODY_WIDTH/2)*(leg_x + BODY_WIDTH/2));
	float y_len = leg_y + BODY_LENGTH/2;
	float x_len = leg_x + BODY_WIDTH/2;
	pointA.fromCartesian(xy_len, leg_z);
	Serial.print("Angle : ");
	Serial.println((pointA.getAngle() * 180/PI));
	Serial.print("r : ");
	Serial.println(pointA.getR());
	float newAngle;
//	if(legType == 1){
		newAngle = (pointA.getAngle() * 180/PI) + angle;
//	}
//	else{
//		newAngle = (pointA.getAngle() * 180/PI) - angle;
//	}
	pointB.fromPolar(pointA.getR(), (newAngle) * PI/180);
	float Bxy = pointB.getX() - BODY_WIDTH/2;
	float Bz = pointB.getY();
	float By = (pointB.getX()*y_len)/xy_len - BODY_LENGTH/2;
	float Bx = (pointB.getX()*x_len)/xy_len - BODY_WIDTH/2;

	Serial.print("pointB XY: ");
	Serial.print(pointB.getX());
	Serial.print(" ");
	Serial.println(pointB.getY());

	Serial.print(leg_x);
	Serial.print("\t");
	Serial.print(leg_y);
	Serial.print("\t");
	Serial.print(leg_z);
	Serial.print("\t Bxy: ");
	Serial.print(Bxy);
	Serial.print("\t Bx: ");
	Serial.print(Bx);
	Serial.print("\t By: ");
	Serial.print(By);
	Serial.print("\t Bz: ");
	Serial.println(Bz);
	move_point(Bx, By, Bz);
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
	legBL.move_point(DEFAULT_LEG_X, DEFAULT_LEG_Y, -1 * h);
	legBR.move_point(DEFAULT_LEG_X, DEFAULT_LEG_Y, -1 * h);
	legFL.move_point(DEFAULT_LEG_X, DEFAULT_LEG_Y, -1 * h);
	legFR.move_point(DEFAULT_LEG_X, DEFAULT_LEG_Y, -1 * h);
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

void Spider::rotate(float angle){
	legBL.rotateZ(angle);
	legBR.rotateZ(angle);
	legFL.rotateZ(angle);
	legFR.rotateZ(angle);
	this->startMove();
	this->march();
}

void Spider::march()
{
	int h1 = -40;
	int h2 = -1 * this->height;
	int spd = legBL.foot.mSpeed;
	setSpeedForAllServos(200);
	legBL.move_point(DEFAULT_LEG_X, DEFAULT_LEG_Y, h1);
	legFR.move_point(DEFAULT_LEG_X, DEFAULT_LEG_Y, h1);
	this->startMove();
	legBL.move_point(DEFAULT_LEG_X, DEFAULT_LEG_Y, h2);
	legFR.move_point(DEFAULT_LEG_X, DEFAULT_LEG_Y, h2);
	this->startMove();

	legBR.move_point(DEFAULT_LEG_X, DEFAULT_LEG_Y, h1);
	legFL.move_point(DEFAULT_LEG_X, DEFAULT_LEG_Y, h1);
	this->startMove();
	legBR.move_point(DEFAULT_LEG_X, DEFAULT_LEG_Y, h2);
	legFL.move_point(DEFAULT_LEG_X, DEFAULT_LEG_Y, h2);
	this->startMove();

	setSpeedForAllServos(spd);
}

void Spider::move2(int dist)
{
	int h = -1 * this->height;
	int dh = this->dhStep;
	int dh2 = this->dhStep2;
//1
	this->step2(1, 0, dist);  //BL
//2
	this->step2(3, 0, dist);  //FR
	legBL.move_point(legBL.leg_x, legBL.leg_y, h);
	legBR.move_point(legBR.leg_x, legBR.leg_y, h);
	legFR.move_point(legFR.leg_x, legFR.leg_y, h);
	legFL.move_point(legFL.leg_x, legFL.leg_y, h);
	this->startMove();
	this->bodyMove(0, -dist, this->height);
//3
	this->step2(2, 0, 2*dist);  //BR

//4
	this->step2(4, 0, 2*dist);  //FL
	legBL.move_point(legBL.leg_x, legBL.leg_y, h);
	legBR.move_point(legBR.leg_x, legBR.leg_y, h);
	legFR.move_point(legFR.leg_x, legFR.leg_y, h);
	legFL.move_point(legFL.leg_x, legFL.leg_y, h);
	this->startMove();

	this->bodyMove(0, -dist, this->height);
//5
	this->step2(1, 0, dist);  //BL
//6
	this->step2(3, 0,dist);  //FR
	legBL.move_point(legBL.leg_x, legBL.leg_y, h);
	legBR.move_point(legBR.leg_x, legBR.leg_y, h);
	legFR.move_point(legFR.leg_x, legFR.leg_y, h);
	legFL.move_point(legFL.leg_x, legFL.leg_y, h);
	this->startMove();
}

void Spider::move(int distX, int distY)
{
	int h = -1 * this->height;
//1
	legBL.move_point(DEFAULT_LEG_X, DEFAULT_LEG_Y, -1 * h);
	this->step2(1, distX, distY);  //BL
//2
	legFL.move_point(DEFAULT_LEG_X, DEFAULT_LEG_Y, -1 * h);
	this->step2(4, distX, distY);  //FL
	legBL.move_point(legBL.leg_x, legBL.leg_y, h);
	legBR.move_point(legBR.leg_x, legBR.leg_y, h);
	legFR.move_point(legFR.leg_x, legFR.leg_y, h);
	legFL.move_point(legFL.leg_x, legFL.leg_y, h);
	this->startMove();
	this->bodyMove(-distX, -distY, this->height);
//3
	this->step2(2, 2*distX, 2*distY);  //BR

//4
	this->step2(3, 2*distX, 2*distY);  //FR
	legBL.move_point(legBL.leg_x, legBL.leg_y, h);
	legBR.move_point(legBR.leg_x, legBR.leg_y, h);
	legFR.move_point(legFR.leg_x, legFR.leg_y, h);
	legFL.move_point(legFL.leg_x, legFL.leg_y, h);
	this->startMove();

	this->bodyMove(-distX, -distY, this->height);
}

void Spider::step(int legInd, float dist)
{
	int h = -1 * this->height;
	int dh = this->dhStep;
	int dh2 = this->dhStep2;
	//расчет индексов
	int indDiag = (legInd<3)?legInd+2:legInd-2;
	int indLeft = (legInd==1)?4:legInd-1;
	int indRight = (legInd==4)?1:legInd+1;
	//Получение ссылок на объекты ног
	Leg *legStep = this->getLegByInd(legInd);
	Leg *legDiag = this->getLegByInd(indDiag);
	Leg *legLeft = this->getLegByInd(indLeft);
	Leg *legRight = this->getLegByInd(indRight);

	if(legInd == 1 || legInd == 2)
	{
		dist *= -1;
	}
	legDiag->move_point(legDiag->leg_x, legDiag->leg_y, h + dh2);
	legLeft->move_point(legLeft->leg_x, legLeft->leg_y, h + dh2);
	legRight->move_point(legRight->leg_x, legRight->leg_y, h - dh2);
	this->startMove();
	legStep->move_point(legStep->leg_x, legStep->leg_y + dist, h + dh); //BL
	this->startMove();
}
void Spider::step2(int legInd, float distX, float distY)
{
	int h = -this->height;
	int dh = this->dhStep;

	Leg *legStep = this->getLegByInd(legInd);

	if(legInd == 1 || legInd == 2)
	{
		distY = -distY;
	}
	if(legInd == 1 || legInd == 4)
	{
		distX = -distX;
	}

	legStep->move_point(legStep->leg_x + distX, legStep->leg_y + distY, h + dh);
	this->startMove();
	legStep->move_point(legStep->leg_x, legStep->leg_y, h);
	this->startMove();
}
void Spider::bodyMove(float x, float y, float z)
{
	legBL.move_point(legBL.leg_x - x, legBL.leg_y - y, 0 - z);
	legBR.move_point(legBR.leg_x + x, legBR.leg_y - y, 0 - z);
	legFL.move_point(legFL.leg_x - x, legFL.leg_y + y, 0 - z);
	legFR.move_point(legFR.leg_x + x, legFR.leg_y + y, 0 - z);
	this->startMove();
//	legBL.leg_x += x; legBL.leg_y += y; legBL.leg_z = 0 - z;
//	legBR.leg_x -= x; legBR.leg_y += y; legBR.leg_z = 0 - z;
//	legFL.leg_x += x; legFL.leg_y -= y; legFL.leg_z = 0 - z;
//	legFR.leg_x -= x; legFR.leg_y -= y; legFR.leg_z = 0 - z;
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
		delay(REFRESH_INTERVAL / 2000); // optional 20 ms delay - REFRESH_INTERVAL is in Microseconds
	} while (!updateAllServos());
}
Leg * Spider::getLegByInd(int ind)
{
	if(legBL.ind == ind)
		return &legBL;
	if(legBR.ind == ind)
		return &legBR;
	if(legFL.ind == ind)
		return &legFL;
	if(legFR.ind == ind)
		return &legFR;
	else
		return &legBL;
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
