//include Servo and 4WheelDrive Library
#include <Servo.h>
#include <AV4Wheel2.h>

//Creating 4WheelDrive object
AV4Wheel2 test;

int buttonPin = 8;

int servoCent = 96;

int addAngle = 25;
int subAngle = 22;

int maxSpeed = 100;

boolean moveAround = false;

void setup(){
    // Parameters: Motor A dirn pin, Motor A speed pin, Motor B dirn pin, Motor B speed pin, 
    // 			   Steering Servo pin, Wheel Circumfrenc (in inches)
    // IMPORTANT: switch Motor B pin wiring manually
    test.init(4, 3, 7, 5, 6, 10.0);
    
    pinMode(buttonPin, INPUT_PULLUP);

    test.initUltra(12,13,500);
    
    Serial.begin(9600);
    
    // Attach interrupt on pin 2
    attachInterrupt(0, interruptFunc, RISING);
    
           // test.testInterrupt();
}

void loop(){
  if(digitalRead(buttonPin) == LOW){
	// ramp up
	// start speed, final speed, delay, steps, motor direction, servo angle
	test.rampMotion(0, maxSpeed, 20, 1, HIGH, servoCent);
	
    test.moveUltra(maxSpeed, 1, servoCent, 54, 1);

    if(moveAround){
      test.moveDist(5.0*12.0, HIGH, maxSpeed, servoCent+addAngle);
      test.moveDist(10.0*12.0, HIGH, maxSpeed, servoCent-subAngle);
      test.moveDist(5.0*12.0, HIGH, maxSpeed, servoCent+addAngle);
        
    }

	// ramp down
	test.rampMotion(maxSpeed, 0, 20, 1, HIGH, servoCent);
  }
}

// need this because interrupt function has to be of void(*)()
void interruptFunc() {
    test.interrupEncoderFunc();
}
