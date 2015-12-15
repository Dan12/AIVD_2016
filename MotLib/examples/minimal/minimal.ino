// minimal motlib code

//MotLib depends on these libraries
#include "Servo.h"
#include "NewPing.h"
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"

#include "MotLib.h"

MotLib testMot;

motorPin1 = -1;
motorPin2 = -1;

void setup(){
  Serial.begin(9600);
  
  testMot.init(motorPin1, motorPin2);
}

void loop(){
  
}