/*
  AV4Wheel-4Wheel Drive library for AIVD
  Daniel Weber 2016
*/

// Make sure that AV4Wheel has not bee defined yet
#ifndef AV4Wheel_h
#define AV4Wheel_h

// Include servo for access to servo library
// Note: only core functions can be included in the library like this
#include "Arduino.h"
#include "Servo.h"

class AV4Wheel{
    public:
        AV4Wheel();
        void init(int md, int ms, int ep, int sp, float wc);
        
        // Interupt functions
        // interupt function to be passed in main program
        void interrupEncoderFunc();
        // method to get interupt ticks
        int getInterrupTicks();
        void testInterrupt();
        void createInterupt(int i);
        
    private:
        int _motorDirn;     // Motor dirn pin
        int _motorSpeed;    // Motor Speed pin
        int _encoderPin;    // Encoder pin
        int _servoPin;      // Servo Pin
        int _encoderPrevVal;    // Encoder Pin Prev Value
        float _wheelCircumfrence;   // Wheel Circumfrence
        Servo _steeringServo;   // Front servo object
        volatile int _interruptTickCounter; // interup ticks counter
        
        // Newping Functions
        int _ping_in();
        uint8_t _triggerBit;
        uint8_t _echoBit;
        volatile uint8_t *_triggerOutput;
        volatile uint8_t *_triggerMode;
        volatile uint8_t *_echoInput;
        boolean _usingInterruptEncoder;
        unsigned int _maxEchoTime;
        unsigned long _max_time;
        boolean ping_trigger();
};

#endif