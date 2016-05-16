// foxtrot params
int curColor = -1;
boolean starting = false;
boolean canStop = false;

void runFoxtrot(){
  if(starting){
    int data1 = bluetooth.parseInt();
    int data2 = bluetooth.parseInt();
    int data3 = bluetooth.parseInt();
    curColor = data2;  
    // number 1 (green)
    if(curColor == 0){
        car.setServo(servoCent);
    }
    // number 2 (yellow)
    else if(curColor == 1){
      car.setServo(servoCent+25);
      car.moveDist(3*12.0, 1, speedAt, servoCent+31);
      car.moveDist(1*12.0, 1, speedAt, servoCent);
    }
    // number 3 (red)
    else if(curColor == 2){
      car.setServo(servoCent-27);
      car.moveDist(4.0*12.0, 1, speedAt, servoCent-27);
      car.moveDist(1*12.0, 1, speedAt, servoCent);
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
