//include Servo and 4WheelDrive Library
#include <Servo.h>
#include <AV4Wheel2.h>

//Creating 4WheelDrive object
AV4Wheel2 car;

int buttonPin = 8;

int servoCent = 96;

int subAngle = 18;
int addAngle = 15;
int maxSpeed = 75;

void setup(){
    // Parameters: Motor A dirn pin, Motor A speed pin, Motor B dirn pin, Motor B speed pin, 
    // 			   Steering Servo pin, Wheel Circumfrenc (in inches)
    // IMPORTANT: switch Motor B pin wiring manually
    car.init(4, 3, 7, 5, 6, 10.0*3.14);
    
    pinMode(buttonPin, INPUT_PULLUP);
    
    Serial.begin(9600);
    
	// Attach interrupt on pin 2
	attachInterrupt(0, interruptFunc, RISING);
	
	// set servo to side
	car.initUltra(A2, A3, 500);
	
	car.setServo(servoCent);
    
	// test.testInterrupt();
}

void loop(){
	if(digitalRead(buttonPin) == LOW){
		// ramp up
		// start speed, final speed, delay, steps, motor direction, servo angle
		car.rampMotion(0, maxSpeed, 20, 1, HIGH, servoCent);

    //fwd till begin of first car
		car.moveUltra(maxSpeed, HIGH, servoCent, 24, 1);
		//fwd util end of first car
		car.moveUltra(maxSpeed, HIGH, servoCent, 30, 0);
		//fwd util begining of second car and slow down
		car.moveUltra(maxSpeed, HIGH, servoCent, 24, 1);
		car.rampMotion(maxSpeed, 0, 20, 1, HIGH, servoCent);
		
		// set servo to back
		car.initUltra(A0, A1 ,500);
		
		//bkwd to halfway between cars
		car.rampMotion(0, maxSpeed, 20, 1, LOW, servoCent-subAngle);
		car.setServo(servoCent-subAngle);
		car.moveDist(3.0*12.0, LOW, maxSpeed, servoCent-subAngle);
		
		//bkwd to end of car
    car.setServo(servoCent+addAngle);
		car.moveUltra(maxSpeed, LOW, servoCent+addAngle, 30, 1);
		car.rampMotion(maxSpeed, 0, 20, 1, LOW, servoCent+addAngle);
		
		// fwd to bumper of front car
		car.initUltra(12,13,500);
		car.setServo(servoCent-subAngle);
		car.rampMotion(0, maxSpeed, 20, 1, HIGH, servoCent-subAngle);
		car.moveUltra(maxSpeed, HIGH, servoCent-subAngle, 30, 1);
		car.rampMotion(maxSpeed, 0, 20, 1, HIGH, servoCent);
		car.setServo(servoCent);
	}
}

// need this because interrupt function has to be of void(*)()
void interruptFunc() {
    car.interrupEncoderFunc();
}
