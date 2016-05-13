#include <SoftwareSerial.h>
#include <Servo.h>
#include <AV4Wheel2.h>

int TxD = 9;
int RxD = 10;

SoftwareSerial bluetooth(TxD, RxD);

AV4Wheel2 aivdCar;

const int center = 96;
const int addAngle = 21;
const int subAngle = 25.5;
const int buttonPin = 8;
const int buttonInputType = INPUT_PULLUP;
const int maxSpeed = 50;
const int exaggerate = 2;

int maxDistance = 200;
int distance = maxDistance;
int stoppingDistance = 26;

int speedAt = 0;
int speedChange = 1;
int speedChangeTimes = 3;
int speedChangeDelay = 20;

boolean isMoving = false;

void setup(){
  //Setup usb serial connection to computer
  Serial.begin(9600);
  Serial.println("Begin");
  //Setup Bluetooth serial connection to android
  bluetooth.begin(38400);
//  bluetooth.print("$$$");
delay(100);
//  bluetooth.println("U,9600,N");
//  bluetooth.begin(9600);
  bluetooth.setTimeout(30);
  
  //Parameters: Motor pin a, Encoder pin, Steering Servo pin, Wheel Circumfrenc (in inches)
  aivdCar.init(4, 3, 7, 5, 6, 10.0*3.14);
  aivdCar.setServo(center);
  //Parameters: Trigger Pin, Echo Pin, Max Distance (cm)
  aivdCar.initUltra(12, 13, maxDistance);
  pinMode(buttonPin,buttonInputType);
}

void loop()
{
  if(digitalRead(buttonPin) == LOW && !isMoving){
    isMoving = true;
    aivdCar.rampMotion(0, maxSpeed, 20, 1, HIGH,  center);
    speedAt = maxSpeed;
  }
  //Read from bluetooth and write to usb serial
  if(bluetooth.available()){

    int centerX = bluetooth.parseInt();
    Serial.println("*");
    Serial.println(centerX);
    Serial.println(distance);
    Serial.println("*");
    int windowWidth = bluetooth.parseInt();
    //Serial.println(data2);
    int servoAngle = center;
    if(centerX < windowWidth/2)
      servoAngle = map(centerX,0,windowWidth/2,center,center+addAngle);
    else
      servoAngle = map(centerX,0,windowWidth/2,center-subAngle,center);
      
    servoAngle = constrain(servoAngle,center-subAngle,center+addAngle);

    aivdCar.setServo(servoAngle);
    
    if(isMoving){
      //go
      if((distance > stoppingDistance || distance == 0) && centerX != -1){
        if(speedAt == maxSpeed)
          aivdCar.genMove(HIGH,maxSpeed);
        else{
          for(int i = 0; i < speedChangeTimes; i++){
            speedAt+=speedChange;
            if(speedAt>maxSpeed)
              speedAt = maxSpeed;
            aivdCar.genMove(HIGH,speedAt);
            delay(speedChangeDelay);
          }
        }
      }
      //stop
      else{
        if(speedAt == 0)
          aivdCar.genMove(LOW,0);
        else{
          for(int i = 0; i < speedChangeTimes; i++){
            speedAt-=speedChange;
            if(speedAt<0)
              speedAt = 0;
            aivdCar.genMove(HIGH,speedAt);
            delay(speedChangeDelay);
          }
        }
      }
    }
  }
  distance = aivdCar.ping_in();
}
