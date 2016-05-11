//include Servo and 4WheelDrive Library
#include <Servo.h>
#include <AV4Wheel2.h>

#include <Wire.h>
#include <LSM303.h>

//Creating 4WheelDrive object
AV4Wheel2 test;

int buttonPin = 8;

int servoCent = 96;

LSM303 compass;

void setup(){
    // Parameters: Motor A dirn pin, Motor A speed pin, Motor B dirn pin, Motor B speed pin, 
    // 			   Steering Servo pin, Wheel Circumfrenc (in inches)
    // IMPORTANT: switch Motor B pin wiring manually
    test.init(4, 3, 7, 5, 6, 10.0*2*3.14);
    
    pinMode(buttonPin, INPUT_PULLUP);

Wire.begin();
  compass.init();
  compass.enableDefault();

  compass.m_min = (LSM303::vector<int16_t>){-741, -679, -540};
  compass.m_max = (LSM303::vector<int16_t>){-1, +230, +599};
    
    Serial.begin(9600);
    
    // Attach interrupt on pin 2
    attachInterrupt(0, interruptFunc, RISING);
test.setServo(servoCent);
    test.initCompass(getCompassHeading);
    test.initPID(0.5,0.5,0.5);
           // test.testInterrupt();
}

void loop(){
  if(digitalRead(buttonPin) == LOW){
	// ramp up
	// start speed, final speed, delay, steps, motor direction, servo angle
 compass.read();
 test.setPIDHeading(compass.heading());
	test.rampMotion(0, 125, 20, 1, HIGH, servoCent);
  test.startPID();
  test.moveDist(12.5*12.0, 1, 125, servoCent);
  
  //test.moveDist(2.5*12.0, 1, 125, servoCent);
  
   test.viewPID();
   test.stopPID();

   //ccw
 //test.moveDist(9.1*12.0, 1, 125, servoCent+25);
 //test.moveDist(8.5*12.0, 1, 125, servoCent-2);
 //test.moveDist(18.1*12.0, 1, 125, servoCent-20);
	// ramp down
	test.rampMotion(125, 0, 20, 1, HIGH, servoCent+2);
  }
}

// need this because interrupt function has to be of void(*)()
void interruptFunc() {
    test.interrupEncoderFunc();
}

float* getCompassHeading(){
    compass.read();
    
    float heading = compass.heading();
    
    return &heading;
}

