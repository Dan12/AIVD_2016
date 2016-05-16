#include <SoftwareSerial.h>

#include <TinyGPS.h>

#include <Wire.h>
#include <LSM303.h>

//include Servo and 4WheelDrive Library
#include <Servo.h>
#include <AV4Wheel2.h>

//Creating 4WheelDrive object
AV4Wheel2 aivdCar;

LSM303 compass;

float heading = -1;
int averageNum = 50;
// 0 = no low pass filter
float alpha = 0;
int headingRange = 1;

TinyGPS gps;
SoftwareSerial ss(11, 11);

float gotolat = 42.300304;
float gotolong = -83.697456;

float xpos = 0.0;
float ypos = 0.0;

int angleTrim = 77;

const int center = 97;
const int addAngle = 25;
const int subAngle = 22;
const int buttonPin = 8;
const int buttonInputType = INPUT_PULLUP;
const int maxSpeed = 100;
const int exaggerate = 2;

static void smartdelay(unsigned long ms);
static void print_float(float val, float invalid, int len, int prec);

boolean moving = false;

boolean done = false;

void setup()
{
  Serial.begin(57600);
  
  ss.begin(9600);

  //aivdCar.init(4, 3, 7, 5, 6, 10.0*3.14);
  //aivdCar.setServo(center);
  
  attachInterrupt(0, interruptFunc, RISING);

  pinMode(buttonPin, INPUT_PULLUP);

  // compass
  Wire.begin();
  compass.init();
  compass.enableDefault();
  
  /*
  Calibration values; the default values of +/-32767 for each axis
  lead to an assumed magnetometer bias of 0. Use the Calibrate example
  program to determine appropriate values for your particular unit.
  */
  
  //compass.m_min = (LSM303::vector<int16_t>){-669, -675, -730};
  //compass.m_max = (LSM303::vector<int16_t>){+744, +592, +510};
  compass.m_min = (LSM303::vector<int16_t>){-673, -754, -855};
  compass.m_max = (LSM303::vector<int16_t>){754, 708, 533};

}
void loop()
{
  if(!done){
    float flat, flon;
  
    long unsigned int age;
  
    gps.f_get_position(&flat, &flon, &age);
    Serial.print(flat);
    Serial.print(",");
    Serial.print(flon);
    
    Serial.println();
    
    smartdelay(1000);
  
    if(digitalRead(buttonPin) == LOW){
    
      xpos = (float)(gotolong-flon);
      ypos = (float)(gotolat-flat);
      convertdeg();
  
      float gotoangle = (float)(atan2((float)ypos,(float)xpos) * (180 / PI) + 180);
      gotoangle -= 180;
      if (gotoangle<0){
        gotoangle+=360;
      }
  
      getCompassHeading();
      int curangle = ((int)heading+angleTrim)%360;
  
      Serial.print("X Distance(feet): ");
      Serial.println(xpos);
      Serial.print("Y Distance(feet): ");
      Serial.println(ypos);
      Serial.print("Goto angle(degrees): ");
      Serial.println(gotoangle);
  
      Serial.print("Degrees to turn: ");
      if (gotoangle>curangle){
        Serial.print(gotoangle-curangle);
        Serial.println(" Clockwise");
      }
      else {
        Serial.print(curangle-gotoangle);
        Serial.println(" Counter Clockwise");
      }
  
      //distance in feet
      float distance = sqrt((((float)(xpos))*((float)(xpos)))+(((float)(ypos))*((float)(ypos))));
      Serial.print("Distance (inches): ");
      Serial.println(distance*12.0);
      
      Serial.println("\\");
      done = true;
      aivdCar.init(4, 3, 7, 5, 6, 10.0*3.14);
      aivdCar.setServo(center);
      if(moving){
        delay(1000);
        aivdCar.setServo(center);
        aivdCar.rampMotion(0,maxSpeed,20,1,HIGH,center);
        //move code here
        if(gotoangle > curangle){
          aivdCar.moveDist(((12.0*7.6)/90.0)*(gotoangle-curangle), HIGH, maxSpeed, center-subAngle);
          aivdCar.moveDist(distance*12.0, HIGH, maxSpeed, center+exaggerate);
        }
        else{
          aivdCar.moveDist(((12.0*7.6)/90.0)*(curangle-gotoangle), HIGH, maxSpeed, center+addAngle);
          aivdCar.moveDist(distance*12.0, HIGH, maxSpeed, center-exaggerate);
        }
      }
    }
  }
  else{
    Serial.println("Done");  
  }
}

static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

static void print_float(float val, float invalid, int len, int prec)
{
  if (val == invalid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartdelay(0);
}

int convertdeg(){
  xpos*=((float)365165.33582);
  ypos*=((float)362776.86915);
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
    
    //Serial.print("Heading: ");
  //Serial.println(heading);
    return &heading;
}

// need this because interrupt function has to be of void(*)()
void interruptFunc() {
  aivdCar.interrupEncoderFunc();
}
