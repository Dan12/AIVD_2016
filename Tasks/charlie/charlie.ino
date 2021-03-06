//include Servo and 4WheelDrive Library
#include <Servo.h>
#include <AV4Wheel2.h>

//Creating 4WheelDrive object
AV4Wheel2 test;

int buttonPin = 8;

int servoCent = 96;

void setup(){
    // Parameters: Motor A dirn pin, Motor A speed pin, Motor B dirn pin, Motor B speed pin, 
    // 			   Steering Servo pin, Wheel Circumfrenc (in inches)
    // IMPORTANT: switch Motor B pin wiring manually
    test.init(4, 3, 7, 5, 6, 10.0*3.14);
    
    pinMode(buttonPin, INPUT_PULLUP);
    
    Serial.begin(9600);
    
	// Attach interrupt on pin 2
	attachInterrupt(0, interruptFunc, RISING);
	
	test.setServo(servoCent);
    
	// test.testInterrupt();
}

void loop(){
	if(digitalRead(buttonPin) == LOW){
		// ramp up
		// start speed, final speed, delay, steps, motor direction, servo angle
		test.rampMotion(0, 125, 20, 1, HIGH, servoCent);

    //straight
		test.moveDist(2.5*12.0, 1, 125, servoCent);
		//ccw
		test.moveDist(9.1*12.0, 1, 125, servoCent+25);
		//straight
    test.moveDist(8.5*12.0, 1, 125, servoCent-2);
    //cw
		test.moveDist(18.1*12.0, 1, 125, servoCent-20);
		// ramp down
		test.rampMotion(125, 0, 20, 1, HIGH, servoCent+2);
	}
}

// need this because interrupt function has to be of void(*)()
void interruptFunc() {
    test.interrupEncoderFunc();
}
