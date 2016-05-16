// follow params
int speedAt = 0;
int speedChange = 1;
int speedChangeTimes = 3;
int speedChangeDelay = 20;
boolean isMoving = false;
int distance = maxDistance;
int stoppingDistance = 26;

void runFollow(){
  if(digitalRead(buttonPin) == LOW && !isMoving){
    isMoving = true;
    car.rampMotion(0, maxSpeed, 20, 1, HIGH,  servoCent);
    speedAt = maxSpeed;
  }

  int centerX = bluetooth.parseInt();
  int windowWidth = bluetooth.parseInt();
  //Serial.println(data2);
  int servoAngle = servoCent;
  if(centerX < windowWidth/2)
    servoAngle = map(centerX,0,windowWidth/2,servoCent,servoCent+addAngle);
  else
    servoAngle = map(centerX,0,windowWidth/2,servoCent-subAngle,servoCent);
    
  servoAngle = constrain(servoAngle,servoCent-subAngle,servoCent+addAngle);

  car.setServo(servoAngle);
  
  if(isMoving){
    //go
    if((distance > stoppingDistance || distance == 0) && centerX != -1){
      if(speedAt == maxSpeed)
        car.genMove(HIGH,maxSpeed);
      else{
        for(int i = 0; i < speedChangeTimes; i++){
          speedAt+=speedChange;
          if(speedAt>maxSpeed)
            speedAt = maxSpeed;
          car.genMove(HIGH,speedAt);
          delay(speedChangeDelay);
        }
      }
    }
    //stop
    else{
      if(speedAt == 0)
        car.genMove(LOW,0);
      else{
        for(int i = 0; i < speedChangeTimes; i++){
          speedAt-=speedChange;
          if(speedAt<0)
            speedAt = 0;
          car.genMove(HIGH,speedAt);
          delay(speedChangeDelay);
        }
      }
    }
  }

  distance = car.ping_in();
}
