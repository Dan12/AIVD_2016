void runCharlie(){
  boolean running = true;
  while(running){
    if(digitalRead(buttonPin) == LOW){
      // ramp up
      // start speed, final speed, delay, steps, motor direction, servo angle
      car.rampMotion(0, 125, 20, 1, HIGH, servoCent);
  
      //straight
      car.moveDist(2.5*12.0, 1, 125, servoCent);
      //ccw
      car.moveDist(9.1*12.0, 1, 125, servoCent+25);
      //straight
      car.moveDist(8.5*12.0, 1, 125, servoCent-2);
      //cw
      car.moveDist(18.1*12.0, 1, 125, servoCent-20);
      // ramp down
      car.rampMotion(125, 0, 20, 1, HIGH, servoCent+2);
      
      running = false;
    }
  }

  returnString = "Finished Charlie";
}
