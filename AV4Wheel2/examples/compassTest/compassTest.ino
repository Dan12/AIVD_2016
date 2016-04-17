//include Servo and 4WheelDrive Library
#include <Servo.h>
#include <AV4Wheel2.h>

//Creating 4WheelDrive object
AV4Wheel2 test;

void setup(){
    // Parameters: Motor A dirn pin, Motor A speed pin, Motor B dirn pin, Motor B speed pin, 
    // 			   Steering Servo pin, Wheel Circumfrenc (in inches)
    // IMPORTANT: switch Motor B pin wiring manually
    test.init(8, 9, 10, 11, 3, 10.0);
    
    Serial.begin(9600);
    
    test.initCompass(getHeading);
}

void loop(){
    
}

// Heading function
int16_t* getHeading(){
  int16_t arr = 140;
  return arr;
}
