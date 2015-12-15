/*
  MotLib-Motorcycle Drive library for AIVD
*/

#include "Arduino.h"
#include "MotLib.h"

#define TICKS_PER_ROTATION 90

MotLib::MotLib(){}

void MotLib::init(int m1a, int m1b, int m2a, int m2b){
    _motor1A = m1a;
    _motor1B = m1b;
    _motor2A = m2a;
    _motor2B = m2b;
    
    pinMode(_motor1A,OUTPUT);
    pinMode(_motor1B,OUTPUT);
    pinMode(_motor2A,OUTPUT);
    pinMode(_motor2B,OUTPUT);
    
    _generalSetup();
}

void MotLib::init(int m1, int m2){
    _motor1A = m1;
    _motor2A = m2;
    
    //used to check if 1 or two pin motor
    _motor1B = -1;
    
    pinMode(_motor1A,OUTPUT);
    pinMode(_motor2A,OUTPUT);
    
    _generalSetup();
}

void MotLib::_generalSetup(){
    _ultra1 = NewPing(9,10,200);
    
    Wire.begin();
    Serial.println("Initialize MPU");
    mpu.initialize();
    Serial.println(mpu.testConnection() ? "Connected" : "Connection failed");
}

void MotLib::_getMPUVals(int16_t vals[]){
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  vals[0] = ax;
  vals[1] = ay;
  vals[2] = az;
  vals[3] = gx;
  vals[4] = gy;
  vals[5] = gz;
}