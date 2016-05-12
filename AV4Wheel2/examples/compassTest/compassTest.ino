#include <Wire.h>
#include <LSM303.h>

//include Servo and 4WheelDrive Library
#include <Servo.h>
#include <AV4Wheel2.h>

//Creating 4WheelDrive object
AV4Wheel2 test;

LSM303 compass;

float heading = -1;
int averageNum = 15;
// 0 = no low pass filter
float alpha = 0;
int headingRange = 2;

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
  
  compass.m_min = (LSM303::vector<int16_t>){-669, -675, -730};
  compass.m_max = (LSM303::vector<int16_t>){+744, +592, +510};
  
  // Parameters: Motor A dirn pin, Motor A speed pin, Motor B dirn pin, Motor B speed pin, 
  //         Steering Servo pin, Wheel Circumfrenc (in inches)
  // IMPORTANT: switch Motor B pin wiring manually
  test.init(4, 3, 7, 5, 6, 7.5*3.14);
}

void loop() {
  long st = millis();
  getCompassHeading();
  Serial.println((millis()-st));
}

float* getCompassHeading(){
  float tempHeading = 0;
  for(int i = 0; i < averageNum; i++){
    compass.read();
    tempHeading += compass.heading();
    }
    tempHeading = tempHeading/averageNum;
    
    if(heading == -1)
      heading = tempHeading;
    else
      heading = heading*alpha + tempHeading*(1-alpha);

    heading = (round(heading)/headingRange)*headingRange;
    
    Serial.print("Heading: ");
  Serial.println(heading);
    return &heading;
}
