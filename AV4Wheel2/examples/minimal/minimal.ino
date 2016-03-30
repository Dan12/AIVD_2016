//include Servo and 4WheelDrive Library
#include <Servo.h>
#include <AV4Wheel2.h>

//Creating 4WheelDrive object
AV4Wheel2 test;

void setup(){
    // Parameters: Motor dirn pin, Motor speed pin, Encoder pin, Steering Servo pin, Wheel Circumfrenc (in inches)
    test.init(8, 9, 2, 3, 10.0);
    
    Serial.begin(9600);
    
    // Attach interrupt on pin 2
    attachInterrupt(0, interruptFunc, RISING);
}

void loop(){
	// ramp up
	// start speed, final speed, motor direction, servo angle
	test.rampMotion(0, 125, HIGH, 90);
	
	// ramp down
	test.rampMotion(125, 0, HIGH, 90);

}

// need this because interrupt function has to be of void(*)()
void interruptFunc() {
    test.interrupEncoderFunc();
}
