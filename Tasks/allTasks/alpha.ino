void runAlpha(){
  Serial.println("Running Alpha");
  boolean running = true;
  while(running){
    if(digitalRead(buttonPin) == LOW){
      delay(5500);
      // ramp up
      // start speed, final speed, delay, steps, motor direction, servo angle  
      car.rampMotion(0, 125, 20, 1, HIGH, servoCent);
  
      car.moveDist(7.7*12.0, 1, 125, servoCent);
      car.moveDist(7.5*12.0, 1, 125, servoCent-27);
      car.moveDist(3.6*12.0, 1, 125, servoCent);
  
      // ramp down
      car.rampMotion(125, 0, 20, 1, HIGH, servoCent+2);

      running = false;
    }
  }

  returnString = "Finished Alpha";
}
