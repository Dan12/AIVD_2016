void runBravo(){
  boolean running = true;
  while(running){
    if(digitalRead(buttonPin) == LOW){
    
      // ramp up
      // start speed, final speed, delay, steps, motor direction, servo angle
      car.rampMotion(0, maxSpeed, 20, 1, HIGH, servoCent);
    
      int tTicks = 28.5*12.0/(10.0*3.14)*90;
      int curDist = car.ping_in();
      car.resetInterruptTicks();
      while(tTicks > car.getInterrupTicks() && (curDist == -1 || curDist > 40)){
        car.genMove(HIGH, maxSpeed); 
        curDist = car.ping_in(); 
      }
    
      // ramp down
      car.rampMotion(maxSpeed, 0, 20, 1, HIGH, servoCent);
      running = false;
    }
  }

  returnString = "Finished Bravo";
}

