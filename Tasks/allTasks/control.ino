// control params
int knobX = 0;
int knobY = 0;
int prevSpeed = 0;
int maxSpeedUp = 3;
boolean prevReverse = false;

void runControl(){
  knobX = bluetooth.parseInt()*-1;
  knobY = bluetooth.parseInt()*-1;

  setMotion();

  returnString = "U:"+String(car.ping_in())+"\nD:"+(car.getInterruptDist()/12);
}

void setMotion(){
  int moveSpeed = constrain(map(knobY,-200,200,-maxSpeed,maxSpeed),-maxSpeed,maxSpeed);
  int moveAngle = servoCent;
  if(knobX < 0)
    moveAngle = constrain(map(knobX,-200,0,servoCent-subAngle,servoCent),servoCent-subAngle,servoCent);
  else
    moveAngle = constrain(map(knobX,0,200,servoCent,servoCent+addAngle),servoCent,servoCent+addAngle);  
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
  //Serial.println(moveAngle);
  car.genMove(reverse ? 0 : 1,moveSpeed);
  
}
