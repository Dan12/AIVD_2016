#include <SoftwareServo.h>
#include "avr/interrupt.h"

SoftwareServo myservo;
int servoPin = 2;
int endcoderIntPin = 1;

int wavesLength = 11;
int wavesBits = 4;
int clockDelay = 20;
int waves[11][4] = {
  {0,0,0,0}, // 0
  {0,0,0,1}, // 1
  {0,0,1,0}, // 2
  {0,0,1,1}, // 3
  {0,1,0,0}, // 4
  {0,1,0,1}, // 5
  {0,1,1,0}, // 6
  {0,1,1,1}, // 7
  {1,0,0,0}, // 8
  {1,0,0,1}, // 9
  {1,1,1,1} // term
};

int curVals[] = {0,0,0,0};
int buffAt = 0;

// pin leg 6
int dataPin = 7;

// pin leg 5
int intPin = 0;
int clockPin = 10;

String curNumber = "";
int command = 0;

int servoCent = 97;
int servoMin = servoCent-34;
int servoMax = servoCent+34;

volatile int interruptTicks = 0;

void setup() {

  GIMSK = 0b00100000;    // turns on pin change interrupts
  PCMSK = 0b00000011;    // turn on interrupts on pins PB0, PB1
  sei();                 // enables interrupts

  myservo.attach(servoPin);
  myservo.setMaximumPulse(2200);
  
  // put your setup code here, to run once:
  startRecieve();

  // encoderInterrupt
  attachInterrupt(endcoderIntPin, tickInterrupt, RISING);
}

void tickInterrupt(){
    interruptTicks++;
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

void startTransmit(){
  command = curNumber.toInt();

  // send ticks
  if(command == 0){
    // start transmission
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    String tickString = String(interruptTicks);
    for(int i = 0; i < sizeof(tickString); i++){
      int trans = tickString.substring(i,i+1).toInt();
      for(int k = 0; k < wavesBits; k++)
        writeDataBit(waves[trans][k]);
    }

    // write terminator
    for(int k = 0; k < wavesBits; k++)
      writeDataBit(waves[wavesLength-1][k]);
      
    startRecieve();
  }
  // don't have to transmit anything
  else{
    // reset ticks
    if(command == 1){
      interruptTicks = 0;
    }
    // set servo
    else{
      if(command >= servoMin && command <= servoMax)
        myservo.write(command);
    }
  }
}

void writeDataBit(int d){
  digitalWrite(clockPin,LOW);
  delayMicroseconds(clockDelay);
  digitalWrite(dataPin,d);
  digitalWrite(clockPin,HIGH);
  delayMicroseconds(clockDelay);
}

void startRecieve(){
  attachInterrupt(intPin, interruptFunc, RISING);
  pinMode(dataPin,INPUT);
}

void newByteArrive(){
  for(int i = 0; i < wavesLength; i++){
    if(waves[i][0] == curVals[0] && waves[i][1] == curVals[1] && waves[i][2] == curVals[2] && waves[i][3] == curVals[3]){
      if(i != 10){
        curNumber+=i;
        break; 
      }
      else{
        startTransmit();  
      }
    }
  }
  
  buffAt = 0;
}

void interruptFunc(){
  curVals[buffAt] = digitalRead(dataPin);
  buffAt++;
  if(buffAt == 4)
    newByteArrive();
}
