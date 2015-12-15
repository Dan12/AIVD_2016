//test servos, motors, ultrasonic sensors, and gyroscope

#include "Servo.h"
#include "NewPing.h"
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"

//define constants
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

//pin variables
int motorPin1 = -1; //Probably 9 and 10 on arduino since at higher frequnency
int motorPin2 = -1;

int wheelServoPin1 = -1;
int wheelServoPin2 = -1;
int stableServoPin = -1;

int triggerPin = -1;
int echoPin = -1;

//object setups
MPU6050 mpu;

Servo wheelServo1;
Servo wheelServo2;
Servo stableServo;

NewPing sonar; // NewPing setup of pins and maximum distance.

void setup() {
  Wire.begin();
  Serial.begin(9600);
   
  Serial.println("Initialize MPU");
  mpu.initialize();
  Serial.println(mpu.testConnection() ? "Connected" : "Connection failed");
  
  sonar = NewPing(triggerPin, ECHO_PIN, MAX_DISTANCE);
  
  wheelServo1.attach(wheelServoPin1);
  wheelServo2.attach(wheelServoPin1);
  stableServo.attach(stableServoPin);
  
  pinMode(motorPin1,OUTPUT);
  pinMode(motorPin2,OUTPUT);
}

void loop() {
  //check mpu
  checkMPU();
  
  //check ultrasonic sensor
  checkPing();
  
  //check servos
  checkServos();
  
  //check motors
  checkMotors();
}

void checkMotors(){
  //1 Pin Motor Duty cycle and equivalent directions (slow-fast): 10-70: stop, 170-90: cw, 200-250: ccw
  analogWrite(motorPin1,100);
  analogWrite(motorPin2,100);
  delay(1000);
  analogWrite(motorPin1,240);
  analogWrite(motorPin2,240);
  delay(1000);
  analogWrite(motorPin1,10);
  analogWrite(motorPin2,10);
  delay(1000);
}

void checkServos(){
  wheelServo1.write(90);
  wheelServo2.write(90);
  stableServo.write(90);
  delay(1000);
  wheelServo1.write(120);
  wheelServo2.write(120);
  stableServo.write(120);
  delay(1000);
  wheelServo1.write(90);
  wheelServo2.write(90);
  stableServo.write(90);
  delay(1000);
}

void checkPing(){
  delay(50);                      // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  Serial.print("Ping: ");
  Serial.print(uS / US_ROUNDTRIP_CM); // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");
}

//get raw readings from mpu
void checkMPU(){
  for(int i = 0; i < 20; i++){
    int16_t ax, ay, az;
    int16_t gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    Serial.print("AX:");
    Serial.print(ax);
    Serial.print(" AY:");
    Serial.print(ay);
    Serial.print(" AZ:");
    Serial.print(az);
    Serial.print(" GX:");
    Serial.print(gx);
    Serial.print(" GY:");
    Serial.print(gy);
    Serial.print(" GZ:");
    Serial.println(gz);
    delay(500);
  }
}