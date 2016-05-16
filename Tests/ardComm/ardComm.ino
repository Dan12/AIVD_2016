int clockDelay = 20;
int wavesBits = 4;
int wavesLength = 11;
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

int interruptTicks = 0;

void setup() {
  // put your setup code here, to run once:
  //startTransmit();
}

void loop() {
  // put your main code here, to run repeatedly:
  setServo(97);
  getInterruptTicks();
  delay(100);
}

void setServo(int angle){
  startTransmit();

  String tickString = String(angle);
  for(int i = 0; i < sizeof(tickString); i++){
    int trans = tickString.substring(i,i+1).toInt();
    for(int k = 0; k < wavesBits; k++)
      writeDataBit(waves[trans][k]);
  }

  // write terminator
  for(int k = 0; k < wavesBits; k++)
    writeDataBit(waves[wavesLength-1][k]);
      
}

void resetInterruptTicks(){
  startTransmit();
  
  // write 1
  for(int k = 0; k < wavesBits; k++)
    writeDataBit(waves[1][k]);

  // write terminator
  for(int k = 0; k < wavesBits; k++)
    writeDataBit(waves[wavesLength-1][k]);

}

void getInterruptTicks(){
  startTransmit();

  // write 0
  for(int k = 0; k < wavesBits; k++)
    writeDataBit(waves[0][k]);

  // write terminator
  for(int k = 0; k < wavesBits; k++)
    writeDataBit(waves[wavesLength-1][k]);

  startRecieve();
  
}

void gotNumber(){
  interruptTicks = curNumber.toInt();
  Serial.print("Int ticks: ");
  Serial.println(interruptTicks);
}

void startTransmit(){
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
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
        gotNumber();  
      }
    }
  }
  
  buffAt = 0;
}

void writeDataBit(int d){
  digitalWrite(clockPin,LOW);
  delayMicroseconds(clockDelay);
  digitalWrite(dataPin,d);
  digitalWrite(clockPin,HIGH);
  delayMicroseconds(clockDelay);
}

void interruptFunc(){
  curVals[buffAt] = digitalRead(dataPin);
  buffAt++;
  if(buffAt == 4)
    newByteArrive();
}
