float gotolat = 42.300304;
float gotolong = -83.697456;

float xpos = 0.0;
float ypos = 0.0;

int angleTrim = -10;

float inchesPerDegree = (12.0*7.6)/90.0;

void runHotel(){
  boolean running = true;
  float heading = bluetooth.parseFloat();
  float flat = bluetooth.parseFloat();
  float flon = bluetooth.parseFloat();
  
  xpos = (float)(gotolong-flon);
  ypos = (float)(gotolat-flat);
  convertdeg();

  float gotoangle = (float)(atan2((float)ypos,(float)xpos) * (180 / PI) + 180);
  gotoangle -= 180;
  if (gotoangle<0){
    gotoangle+=360;
  }

  int curangle = ((int)heading+angleTrim)%360;

  Serial.print("X Distance(feet): ");
  Serial.println(xpos);
  Serial.print("Y Distance(feet): ");
  Serial.println(ypos);
  Serial.print("Goto angle(degrees): ");
  Serial.println(gotoangle);

  Serial.print("Degrees to turn: ");
  if (gotoangle>curangle){
    Serial.print(gotoangle-curangle);
    Serial.println(" Clockwise");
  }
  else {
    Serial.print(curangle-gotoangle);
    Serial.println(" Counter Clockwise");
  }

  //distance in feet
  float distance = sqrt((((float)(xpos))*((float)(xpos)))+(((float)(ypos))*((float)(ypos))));
  Serial.print("Distance (inches): ");
  Serial.println(distance*12.0);
  
  Serial.println("\\");
  if(digitalRead(buttonPin) == LOW){
    delay(1000);
    car.setServo(servoCent);
    car.rampMotion(0,maxSpeed,20,1,HIGH,servoCent);
    //move code here
    if(gotoangle > curangle){
      car.moveDist(inchesPerDegree*(gotoangle-curangle), HIGH, maxSpeed, servoCent-subAngle);
      car.moveDist(distance*12.0, HIGH, maxSpeed, servoCent+exaggerate);
    }
    else{
      car.moveDist(inchesPerDegree*(curangle-gotoangle), HIGH, maxSpeed, servoCent+addAngle);
      car.moveDist(distance*12.0, HIGH, maxSpeed, servoCent-exaggerate);
    }
  }
}

void convertdeg(){
  xpos*=((float)365165.33582);
  ypos*=((float)362776.86915);
}
