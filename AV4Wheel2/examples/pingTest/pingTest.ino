//include Servo and 4WheelDrive Library
#include <Servo.h>
#include <AV4Wheel2.h>

//Creating 4WheelDrive object
AV4Wheel2 test;

void setup(){
    // Parameters: Motor A dirn pin, Motor A speed pin, Motor B dirn pin, Motor B speed pin, 
    // 			   Steering Servo pin, Wheel Circumfrenc (in inches)
    // IMPORTANT: switch Motor B pin wiring manually
    test.init(4, 3, 7, 5, 6, 7.5*3.14);
    
    Serial.begin(9600);
    
    // Attach interrupt on pin 2
    attachInterrupt(0, interruptFunc, RISING);
    
    test.initUltra(12,13,700);
    test.initUltra(A0,A1,700);
    test.initUltra(A2,A3,700);
}

void loop(){
	// ramp up
	// start speed, final speed, delay, steps, motor direction, servo angle
	//test.rampMotion(0, 125, 30, 1, HIGH, 90);
	
	// ramp down
	//test.rampMotion(125, 0, 30, 1, HIGH, 90);
  Serial.print("Inches: ");
  Serial.println(test.ping_in());
  delay(29);
}

// need this because interrupt function has to be of void(*)()
void interruptFunc() {
    test.interrupEncoderFunc();
}
