/*
  AV4Wheel2-4 Wheel Drive library for AIVD
  Daniel Weber 2016
*/

// Make sure that AV4Wheel has not bee defined yet
#ifndef AV4Wheel2_h
#define AV4Wheel2_h

// Include servo for access to servo library
// Note: only core functions can be included in the library like this
#include "Arduino.h"
#include "Servo.h"

class AV4Wheel2{
    public:
        AV4Wheel2();
        void init(int md, int ms, int ep, int sp, float wc);
        void rampMotion(int startSpeed, int finalSpeed, int dirn, int servoAngle);
        void moveDist(int dist, int dirn, int speed, int servoAngle);
        void moveUltra(int speed, int dirn, int servoAngle, int ultraDist, int ultraTrigger);
        
        //Public PID functions
        void initPID();
        void startPID();
        
        // Compass functions
        void initCompass(int16_t* (*func)());
        // set PID heading
        void setPIDHeading(int16_t dirn);
        
        // Interupt functions
        // interupt function to be passed in main program
        void interrupEncoderFunc();
        // method to get interupt ticks
        int getInterrupTicks();
        void testInterrupt();
        
        // Setup Newping Sensor
        void initUltra(uint8_t trigger_pin, uint8_t echo_pin, int max_cm_distance);
        // Get Newping ping in inches
        int ping_in();
        
    private:
        int _motorDirn;     // Motor dirn pin
        int _motorSpeed;    // Motor Speed pin
        int _encoderPin;    // Encoder pin
        int _servoPin;      // Servo Pin
        float _wheelCircumfrence;   // Wheel Circumfrence
        Servo _steeringServo;   // Front servo object
        volatile int _interruptTickCounter; // interup ticks counter
        int16_t _compassHeading;	// Compass heading
        int16_t* (*_headingFunc)();	// Pointer to compass heading function that returns int16_t pointer
		void _getHeading();	// Library function to call compass heading function and store the result
        
        // movement functions
        void _genMove(int dirn, int speed);
        
        // PID variables and methods
        void _logPID();
        int _getAdjustment();
        int _pidStartTime;
        void _adjustServo(int baseAngle);
        
        // Newping Functions
        uint8_t _triggerBit;
        uint8_t _echoBit;
        volatile uint8_t *_triggerOutput;
        volatile uint8_t *_triggerMode;
        volatile uint8_t *_echoInput;
        boolean _usingInterruptEncoder;
        unsigned int _maxEchoTime;
        unsigned long _max_time;
        boolean _ping_trigger();
};

#endif
