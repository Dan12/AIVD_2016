#include "Servo.h"

Servo moveServo1;
Servo moveServo2;

Servo motorServo1;
Servo motorServo2;

int servoPin1 = 9;
int servoPin2 = 10;
int motorPin1 = 6;
int motorPin2 = 5;

void setup()
{
	moveServo1.attach(servoPin1);
	moveServo2.attach(servoPin2);
	
	motorServo1.attach(motorPin1);
	motorServo2.attach(motorPin2);
	
	Serial.begin(9600);
}

// vex motors: use a servo to set up, then use angles to specify direction and speed
// find out these values:
// (slow-fast)  200-250: stop   90-0 (cw)   90-200(ccw)

void loop()
{
  // set dirn to straight
	moveServo1.write(90);
	moveServo2.write(90);

	// motors off
	motorServo1.write(250);
	motorServo2.write(250);
	
	delay(2000);
	
	//to the left and forward
	moveServo1.write(170);
	moveServo2.write(170);
	
	motorServo1.write(150);
	motorServo2.write(30);
	
	delay(1000);
	
	//to the right and forward
	moveServo1.write(10);
	moveServo2.write(10);
	
	delay(1000);
	
}