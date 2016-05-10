#include <Wire.h>
#include <LSM303.h>

//include Servo and 4WheelDrive Library
#include <Servo.h>
#include <AV4Wheel2.h>

//Creating 4WheelDrive object
AV4Wheel2 test;

LSM303 compass;

void setup() {
  Serial.begin(9600);
  
  Wire.begin();
  compass.init();
  compass.enableDefault();
  
  /*
  Calibration values; the default values of +/-32767 for each axis
  lead to an assumed magnetometer bias of 0. Use the Calibrate example
  program to determine appropriate values for your particular unit.
  */
  
  compass.m_min = (LSM303::vector<int16_t>){-420, -528, -455};
  compass.m_max = (LSM303::vector<int16_t>){+846, +420, +716};
  
  // Parameters: Motor A dirn pin, Motor A speed pin, Motor B dirn pin, Motor B speed pin, 
  // 			   Steering Servo pin, Wheel Circumfrenc (in inches)
  // IMPORTANT: switch Motor B pin wiring manually
	test.init(4, 3, 7, 5, 6, 7.5*3.14);
}

void loop() {
  getCompassHeading();
}

float* getCompassHeading(){
    compass.read();
    
    float heading = compass.heading();
    Serial.print("Heading: ");
  Serial.println(heading);
    return &heading;
}

