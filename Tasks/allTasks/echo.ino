void runEcho(){
  boolean running = true;
  while(running){
    if(digitalRead(buttonPin) == LOW){
      // ramp up
      // start speed, final speed, delay, steps, motor direction, servo angle
      car.rampMotion(0, maxSpeed, 20, 1, HIGH, servoCent);
  
      //fwd till begin of first car
      car.moveUltra(maxSpeed, HIGH, servoCent, 24, 1);
      //fwd util end of first car
      car.moveUltra(maxSpeed, HIGH, servoCent, 30, 0);
      //fwd util begining of second car and slow down
      car.moveUltra(maxSpeed, HIGH, servoCent, 24, 1);
      car.rampMotion(maxSpeed, 0, 20, 1, HIGH, servoCent);
      
      // set servo to back
      car.initUltra(A0, A1 ,500);
      
      //bkwd to halfway between cars
      car.rampMotion(0, maxSpeed, 20, 1, LOW, servoCent-subAngle);
      car.setServo(servoCent-subAngle);
      car.moveDist(3.0*12.0, LOW, maxSpeed, servoCent-subAngle);
      
      //bkwd to end of car
      car.setServo(servoCent+addAngle);
      car.moveUltra(maxSpeed, LOW, servoCent+addAngle, 30, 1);
      car.rampMotion(maxSpeed, 0, 20, 1, LOW, servoCent+addAngle);
      
      // fwd to bumper of front car
      car.initUltra(12,13,500);
      car.setServo(servoCent-subAngle);
      car.rampMotion(0, maxSpeed, 20, 1, HIGH, servoCent-subAngle);
      car.moveUltra(maxSpeed, HIGH, servoCent-subAngle, 30, 1);
      car.rampMotion(maxSpeed, 0, 20, 1, HIGH, servoCent);
      car.setServo(servoCent);

      running = false;
    }  
  }

  returnString = "Finished Echo";
}
