#include <SoftwareSerial.h>
#include <Servo.h>
#include <AV4Wheel2.h>

AV4Wheel2 car;

int TxD = 10;
int RxD = 9;

int maxDistance = 200;
int distance = maxDistance;

int knobX = 0;
int knobY = 0;

int prevSpeed = 0;
int maxSpeedUp = 3;
boolean prevReverse = false;

int distScale = 2;

SoftwareSerial bluetooth(TxD, RxD);

boolean returnMes = false;

const int center = 97;
const int addAngle = 21;
const int subAngle = 25.5;
const int buttonPin = 10;
const int buttonInputType = INPUT_PULLUP;
const int maxSpeed = 125;
const int exaggerate = 2;

void setup(){
  //Setup usb serial connection to computer
  Serial.begin(9600);
  Serial.println("Begin");
  //Setup Bluetooth serial connection to android
  bluetooth.begin(115200);
  bluetooth.print("$$$");
  delay(100);
  bluetooth.println("U,9600,N");
  bluetooth.begin(9600);
  bluetooth.setTimeout(30);
  
  //Parameters: Motor pin a, Encoder pin, Steering Servo pin, Wheel Circumfrenc (in inches)
  car.init(4, 3, 7, 5, 6, 7.5*3.14);
  car.setServo(center);
  //Parameters: Trigger Pin, Echo Pin, Max Distance (cm)
  car.initUltra(12, 13, maxDistance);
  
  attachInterrupt(0, interruptFunc, RISING);
}

void loop()
{
  //Read from bluetooth and write to usb serial
  if(bluetooth.available())
  {
    //Serial.println(bluetooth.read());
    int data1 = bluetooth.parseInt();
    //Serial.println(data1);
    int data2 = bluetooth.parseInt();
    //Serial.println(data2);
    knobX = data1*-1;
    knobY = data2*-1;
    Serial.println("*");
    Serial.println(knobX);
    Serial.println(knobY);
    Serial.println("*");
    returnMes = true;
    setMotion();
  }
  else if(returnMes){
    int distance = car.ping_in();
    if(distance == 0)
      distance = maxDistance;
    String distStr = String(distance);
    str = "Distance: "+distStr+"\nHeading: 0.00\nLat: 42.12412\nLong: -87.2431\nIntegral: "+car.getPIDIntegral()+"\nEncoder Ticks: "+car.getInterrupTicks()+"\nEncoder Dist: "+car.getInterrupDist();
    sendStr(str);
    //Serial.println(distance);
    returnMes = false;
  }
}

void setMotion(){
  int moveSpeed = constrain(map(knobY,-200,200,-maxSpeed,maxSpeed),-maxSpeed,maxSpeed);
  int moveAngle = center;
  if(knobX < 0)
    moveAngle = constrain(map(knobX,-200,0,center-subAngle,center),center-subAngle,center);
  else
    moveAngle = constrain(map(knobX,0,200,center,center+addAngle),center,center+addAngle);  
  if(abs(moveSpeed-prevSpeed) > maxSpeedUp){
    if(moveSpeed-prevSpeed < 0)
      moveSpeed = prevSpeed-maxSpeedUp;
    else 
      moveSpeed = prevSpeed+maxSpeedUp;
  }
  prevSpeed = moveSpeed;
  boolean reverse = moveSpeed < 0;
  moveSpeed = abs(moveSpeed);
  //Serial.println(reverse ? 0 : 1);
  car.setServo(moveAngle);
  car.genMove(reverse ? 0 : 1,moveSpeed);
  
}

void sendStr(String str){
  char b[2];
  for(int i = 0; i < str.length(); i++){
    String dataSend = str.substring(i,i+1);
    dataSend.toCharArray(b,2);
    bluetooth.print(b);
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

String floatToString(float val){
	char outstr[25];
	sprintf(outstr, "%f", val);
	return String(outstr);
}
