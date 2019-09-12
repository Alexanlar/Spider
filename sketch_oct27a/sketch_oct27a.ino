#include "I2Cdev.h"
#include "MPU6050.h"
#include <Servo.h>

int ForwardPin = 3;
int BackwardPin = 5;

#define TO_DEG 57.29577951308232087679815481410517033f

#define T_OUT 1 // каждый 20 миллисекунд будем проводить вычисления 
#define P_OUT 10 // каждый 50 миллисекунд будем выводить данные
#define FK 0.3 // коэффициент комплементарного фильтра
#define Kp 0.3 // коэффициент пропорциональной составляющей
#define Ki 0.5 // коэффициент интегральной составляющей
#define Kd 0.5 // коэффициент дифференциальной составляющей
#define Kspd 31  // коэффициент отношения цифрового значения к об/с
#define d 14  // высота до датчика MPU6050
#define C 14.5 // длина окружности колес
float Xstd = -2;

MPU6050 accelgyro;

float angle_ax, angle_ay, angle_az, angle_gx, angle_gy, angle_gz, angle_x, angle_y, angle_z, alfa;
float angle_ex, angle_ox, angle_ex_past;
double spd;
float P, I, D;
int ax_raw, ay_raw, az_raw, gx_raw, gy_raw, gz_raw;
int16_t aclx, acly, aclz, gclx, gcly, gclz;
int dt = 0;
long int t_next, p_next;

Servo myservo;

// функция, которая не даёт значению выйти за пределы 
float clamp(float v, float minv, float maxv){
    if( v>maxv )
        return maxv;
    else if( v<minv )
        return minv;
    return v;
}

void setup() {
    Serial.begin(9600);
  myservo.attach(14);
  pinMode(ForwardPin, OUTPUT);
  pinMode(BackwardPin, OUTPUT);
    // инициализация MPU6050
    accelgyro.initialize();
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
    accelgyro.getMotion6(&ax_raw, &ay_raw, &az_raw, &gx_raw, &gy_raw, &gz_raw);
    aclx = ax_raw;
    acly = ay_raw;
    aclz = az_raw;
    
    gclx = gx_raw;
    gcly = gy_raw;
    gclz = gz_raw;
    alfa = 0.75;
//    Serial.print(aclx);
//    Serial.print("\t");
//    Serial.print(acly);
//    Serial.print("\t");
//    Serial.println(aclz);
//    Serial.println(alfa);
}

void loop() {
    long int t = millis();
    // каждые T_TO миллисекунд выполняем рассчет угла наклона
    if( t_next < t ){

        t_next = t + T_OUT;
        angle_gx = angle_x + (gx_raw/16400)*T_OUT;
        // получаем сырые данные от датчиков в MPU6050
        accelgyro.getMotion6(&ax_raw, &ay_raw, &az_raw, &gx_raw, &gy_raw, &gz_raw);
        //0-90
        if(ax_raw > 0 && az_raw > 0)
          angle_ax = TO_DEG * atan((ax_raw)/(az_raw)) - 90;
        //90-180
        else if(ax_raw > 0 && az_raw < 0)
          angle_ax = TO_DEG * atan((ax_raw)/(az_raw)) + 90;
        //180-270
        else if(ax_raw < 0 && az_raw < 0)
          angle_ax = TO_DEG * atan((ax_raw)/(az_raw)) + 180;
        //270-360
        else if(ax_raw < 0 && az_raw > 0)
          angle_ax = TO_DEG * atan((ax_raw)/(az_raw)) + 270;
        angle_x = FK * angle_gx + (1 - FK)*angle_ax;
    //PID
        angle_ex = angle_x - Xstd;
        P = Kp * angle_ex;
        I = I + Ki * angle_ex;
        D = Kd*(angle_ex - angle_ex_past);
        angle_ex_past = angle_ex;
        angle_ox = P+D+I;
        I = clamp(I, -25, 25);
    }

    t = millis();
    // каждые P_OUT миллисекунд выводим результат в COM порт
    if( p_next < t ){
    //Driver
        spd = 0;
//        spd = tan(angle_ex/TO_DEG)*(d * Kspd/(C * P_OUT/1000));
        spd = angle_ox;
        spd = clamp(spd, -250, 250);
        if(spd > 0)
        {
          spd = abs(spd)+150;
          spd = clamp(spd, -250, 250);
          analogWrite(BackwardPin, spd);
          digitalWrite(ForwardPin, LOW);
        }
        else
        {
          spd = abs(spd)+150;
         spd = clamp(spd, -250, 250);
//          spd = tan(spd)*(d * Kspd/(C * (P_OUT/1000)));
          digitalWrite(BackwardPin, LOW);
          analogWrite(ForwardPin, spd);
          spd = spd * -1;
        }
        p_next = t + P_OUT;
//        Serial.print(angle_ax);
//        Serial.print("\t");
//        Serial.print(angle_gx);
//        Serial.print("\t");
        myservo.write(angle_x);
        Serial.print(angle_x);
        Serial.print("\t");
        Serial.print(angle_ex);
        Serial.print("\t");
        Serial.print(angle_ox);
        Serial.print("\t");
        Serial.println(spd);
    }
}
