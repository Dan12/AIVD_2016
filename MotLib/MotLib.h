/*
  MotLib-Motorcycle Drive library for AIVD
*/

//if motLib not defined yet
#ifndef MotLib_h
#define MotLib_h

#include "Arduino.h"
//arduino libraries
#include "Servo.h"
#include "Wire.h"
//contributed libraries
#include "../NewPing/NewPing.h"
#include "../I2Cdev/I2Cdev.h"
#include "../MPU6050/MPU6050.h"

class MotLib{
  public:
    //constructor
    MotLib();
    
    //init methods
    void init(int m1a, int m1b, int m2a, int m2b);
    void init(int m1, int m2);
  private:
    //motor pins (only b used in 1 pin motor)
    int _motor1A;
    int _motor1B;
    int _motor2A;
    int _motor2B;
    NewPing _ultra1 = NewPing(0,0,0);
    MPU6050 _mpu;
    
    void _generalSetup();
    void _getMPUVals(int16_t vals[]);
};

#endif