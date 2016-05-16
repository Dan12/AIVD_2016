#include <SoftwareSerial.h>
#include <Servo.h>
#include <AV4Wheel2.h>

#include <Wire.h>

AV4Wheel2 car;

int TxD = 9;
int RxD = 10;
int btBaud = 9600;
SoftwareSerial bluetooth(TxD, RxD);

boolean returnMes = false;
String returnString = "Default Return";

const int servoCent = 97;
// should turning the same distance with add angle and sub angle
const int addAngle = 35;
const int subAngle = 29;
const int exaggerate = 2;

const int buttonPin = 8;
const int buttonInputType = INPUT_PULLUP;

const int maxSpeed = 125;
const int maxDistance = 300;

int curMode = 1;

void setup() {
  Serial.begin(9600);
  Serial.println("Begin");
  //Setup Bluetooth serial connection to android
  bluetooth.begin(btBaud);
  delay(100);
  bluetooth.setTimeout(30);

  //Parameters: Motor pin a, Encoder pin, Steering Servo pin, Wheel Circumfrenc (in inches)
  car.init(4, 3, 7, 5, 6, 10.0*3.14);
  car.setServo(servoCent);
  //Parameters: Trigger Pin, Echo Pin, Max Distance (cm)
  car.initUltra(12, 13, maxDistance);
  
  attachInterrupt(0, interruptFunc, RISING);

  pinMode(buttonPin, buttonInputType);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Read from bluetooth and write to usb serial
  if(bluetooth.available())
  {
    //Serial.println(bluetooth.read());
    curMode = bluetooth.parseInt();
    switch(curMode){
      case 1:
        runControl();
        break;
      case 2:
        runAlpha();
        break;
      case 3:
        runBravo();
        break;
      case 4:
        runCharlie();
        break;
      case 5:
        runDelta();
        break;
      case 6:
        runEcho();
        break;
      case 7:
        runHotel();
        break;
      case 8:
        runFoxtrot();
        break;
      case 9:
        runFollow();
        break;
    }
    returnMes = true;
  }
  else if(returnMes){
    sendStr(returnString);
    returnMes = false;
  }
}

void sendStr(String str){
  char b[2];
  for(int i = 0; i < str.length(); i++){
    String dataSend = str.substring(i,i+1);
    dataSend.toCharArray(b,2);
    bluetooth.print(b);
    delay(1);
  }
  String endStr = "*";
  char e[2];
  endStr.toCharArray(e,2);
  bluetooth.print(e);
}

// need this because interrupt function has to be of void(*)()
void interruptFunc() {
  car.interrupEncoderFunc();
}
