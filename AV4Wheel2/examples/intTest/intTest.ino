//include Servo and 4WheelDrive Library
#include <Servo.h>
#include <AV4Wheel2.h>

//Creating 4WheelDrive object
AV4Wheel2 test;

boolean otherTest = true;

void setup(){
    // Parameters: Motor A dirn pin, Motor A speed pin, Motor B dirn pin, Motor B speed pin, 
    // 			   Steering Servo pin, Wheel Circumfrenc (in inches)
    // IMPORTANT: switch Motor B pin wiring manually
    test.init(4, 3, 7, 5, 6, 10.0);
    
    Serial.begin(9600);
    
    // Attach interrupt on pin 2
    attachInterrupt(0, interruptFunc, RISING);
}

void loop(){
    if(otherTest){
        // test interrupt function in object
        test.testInterrupt();
        otherTest = false;
    }
    else{
        // test interrupt function in main program
        Serial.println(test.getInterrupTicks());
    }
}

// need this because interrupt function has to be of type void(*)()
void interruptFunc() {
    test.interrupEncoderFunc();
}
