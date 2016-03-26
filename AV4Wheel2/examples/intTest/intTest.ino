//include Servo and 4WheelDrive Library
#include <Servo.h>
#include <AV4Wheel.h>

//Creating 4WheelDrive object
AV4Wheel test;

boolean otherTest = false;

void setup(){
    //Parameters: Motor dirn pin, Motor speed pin, Encoder pin, Steering Servo pin, Wheel Circumfrenc (in inches)
    test.init(8, 9, 2, 3, 10.0);
    
    Serial.begin(9600);
    
    // Attach interrupt on pin 2
    // attachInterrupt(0, test.interrupEncoderFunc, RISING);
    // attachInterrupt(0, interruptFunc, RISING);
    test.createInterupt(0);
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

// backup
void interruptFunc() {
    test.interrupEncoderFunc();
}