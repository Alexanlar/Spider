#include "I2Cdev.h"
#include "MPU6050.h"
int ForwardPin = 5;
int BackwardPin = 3;

#define TO_DEG 57.29577951308232087679815481410517033f
#define T_OUT 50

MPU6050 accel;

int16_t ax_raw, ay_raw, az_raw, gx_raw, gy_raw, gz_raw;
float angle_ax, angle_ay, angle_az, ax_raw_scalled, ay_raw_scalled ,az_raw_scalled;
long int t_next;
int spd;

float clamp(float v, float minv, float maxv){
    if( v>maxv )
        return maxv;
    else if( v<minv )
        return minv;
    return v;
}

void setup() {
  Serial.begin(9600);
  pinMode(ForwardPin, OUTPUT);
  pinMode(BackwardPin, OUTPUT);
  accel.initialize(); // первичная настройка датчика
  Serial.println(accel.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  accel.getMotion6(&ax_raw, &ay_raw, &az_raw, &gx_raw, &gy_raw, &gz_raw);
  ax_raw_scalled = ax_raw;
  ay_raw_scalled = ay_raw;
  az_raw_scalled = az_raw;
  Serial.println("Calibrate");
  Serial.print(ax_raw);Serial.print(" ");Serial.print(ax_raw_scalled);
  Serial.print("\t");
  Serial.print(ay_raw);Serial.print(" ");Serial.print(ay_raw_scalled); 
  Serial.print("\t");
  Serial.print(az_raw);Serial.print(" ");Serial.println(az_raw_scalled);
  analogWrite(BackwardPin, 50);
  digitalWrite(ForwardPin, LOW);
}

void loop() {
    long int t = millis();
    if( t_next < t ){
        float ax, az, ay, gx;

        t_next = t + T_OUT;
        accel.getMotion6(&ax_raw, &ay_raw, &az_raw, &gx_raw, &gy_raw, &gz_raw);
//        ax_raw = ax_raw - ax_raw_scalled;
//        ay_raw = ay_raw - ay_raw_scalled;
//        az_raw = az_raw - az_raw_scalled;
//         сырые данные акселерометра нужно преобразовать в единицы гравитации
//         при базовых настройках 1G = 4096
        ay = ay_raw / 4096.0;
        ax = ax_raw / 4096.0;
        az = az_raw / 4096.0;
        // на случай, если на акселерометр действуют посторонние силы, которые могут
        // увеличить ускорение датчика свыше 1G, установим границы от -1G до +1G  
        ay = clamp(ay, -1.0, 1.0);
        ax = clamp(ax, -1.0, 1.0);
        az = clamp(az, -1.0, 1.0);

        // функция acos возвращает угол в радианах, так что преобразуем
        // его в градусы при помощи коэффициента TO_DEG
        if( ay >= 0){
            angle_ax = TO_DEG*acos(ay);
            angle_ay = TO_DEG*acos(ax);
            angle_az = TO_DEG*acos(az);
        } else {
            angle_ax = TO_DEG*acos(-ay);
            angle_ay = TO_DEG*acos(-ay);
            angle_az = TO_DEG*acos(-az);
        }
//        if(ay_raw < 2000 && ay_raw > -2000)
//        {
//          digitalWrite(BackwardPin, LOW);
//          digitalWrite(ForwardPin, LOW);
//        }
//        else
//        {
//          if(ay_raw > 0)
//          {
//            spd = ay_raw/100;
//            analogWrite(BackwardPin, spd);
//            digitalWrite(ForwardPin, LOW);
//          }
//          else
//          {
//            spd = abs(ay_raw)/100;
//            digitalWrite(BackwardPin, LOW);
//            analogWrite(ForwardPin, spd);
//          }
//        }
        Serial.print(ax_raw);
        Serial.print("\t");
        Serial.print(ay_raw);
        Serial.print("\t");
        Serial.print(az_raw);
        Serial.print("\t");
        Serial.print(gx_raw);
        Serial.print("\t");
        Serial.print(gy_raw); 
        Serial.print("\t");
        Serial.print(gz_raw);
        Serial.print("\t");
        Serial.print(angle_ax);
        Serial.print("\t");
        Serial.print(angle_ay);
        Serial.print("\t");
        Serial.println(angle_az); // вывод в порт угла поворота вокруг оси X
    }
}
