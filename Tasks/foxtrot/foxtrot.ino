#include <SoftwareSerial.h>
#include <Servo.h>
#include <AV4Wheel2.h>

int TxD = 9;
int RxD = 10;

SoftwareSerial bluetooth(TxD, RxD);

AV4Wheel2 car;

const int servoCent = 103;
const int addAngle = 21;
const int subAngle = 25.5;
const int maxSpeed = 100;
const int exaggerate = 2;

int maxDistance = 200;
int distance = maxDistance;
int stoppingDistance = 30;

int speedChangeDelay = 20;
int speedAt = 0;

int curColor = -1;

boolean starting = false;
boolean canStop = false;

int buttonPin = 8;

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
  car.init(4, 3, 7, 5, 6, 10.0*3.14);
  car.setServo(servoCent);
  //Parameters: Trigger Pin, Echo Pin, Max Distance (cm)

  pinMode(buttonPin, INPUT_PULLUP);

  // Attach interrupt on pin 2
  attachInterrupt(0, interruptFunc, RISING);
}

void loop()
{
  if(starting){
    //Read from bluetooth and write to usb serial
    if(bluetooth.available()){
      int data1 = bluetooth.parseInt();
      int data2 = bluetooth.parseInt();
      int data3 = bluetooth.parseInt();
      curColor = data2;
      Serial.println(curColor);
    }
    // number 1 (green)
    if(curColor == 0){
      //if(speedAt != maxSpeed){
        car.setServo(servoCent);
        //car.rampMotion(speedAt,maxSpeed,20,1,HIGH,servoCent);
        //speedAt = maxSpeed;
      //}
    }
    // number 2 (yellow)
    else if(curColor == 1){
      car.setServo(servoCent+25);
      car.moveDist(3*12.0, 1, speedAt, servoCent+31);
      car.moveDist(1*12.0, 1, speedAt, servoCent);
      car.setServo(servoCent);
//      if(speedAt != maxSpeed/2){
//        car.rampMotion(speedAt,maxSpeed/2,20,1,HIGH,servoCent);
//        speedAt = maxSpeed/2;
//      }
    }
    // number 3 (red)
    else if(curColor == 2){
      car.setServo(servoCent-27);
      car.moveDist(4.0*12.0, 1, speedAt, servoCent-27);
      car.moveDist(1*12.0, 1, speedAt, servoCent);
       car.setServo(servoCent);
      //if(speedAt != 0){
        //car.rampMotion(speedAt,0,20,1,HIGH,servoCent);
        //speedAt = 0;
      //}
    }
    if(canStop && digitalRead(buttonPin) == LOW){
      car.rampMotion(speedAt,0,20,1,HIGH,servoCent);
      speedAt = 0; 
      starting = false;  
      canStop = false;
    }
    else if(!canStop && digitalRead(buttonPin) == HIGH){
        canStop = true;
    }
  }
  else{
      if(digitalRead(buttonPin) == LOW){
        car.rampMotion(0,maxSpeed,20,1,HIGH,servoCent);
        speedAt = maxSpeed; 
        starting = true; 
      }
  }
}

  // need this because interrupt function has to be of void(*)()
void interruptFunc() {
  car.interrupEncoderFunc();
}
