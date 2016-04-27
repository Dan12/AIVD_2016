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
        void init(int mda, int msa, int mdb, int msb, int sp, float wc);
        void rampMotion(int startSpeed, int finalSpeed, int dirn, int servoAngle);
        void moveDist(int dist, int dirn, int speed, int servoAngle);
        void moveUltra(int speed, int dirn, int servoAngle, int ultraDist, int ultraTrigger);
        void changeHeading(int speed, int dirn, int servoAngleCW, int servoAngleCCW, float gotoHeading);
        
        //Public PID functions
        void initPID(float p, float i, float d);
        void startPID();
        void resetPID();
        
        // Compass functions
        void initCompass(float* (*func)());
        // set PID desired heading
        void setPIDHeading(float dirn);
        
        // Interupt functions
        // interupt function to be passed in main program
        void interrupEncoderFunc();
        // method to get interupt ticks
        int getInterrupTicks();
        void testInterrupt();
        void resetInterruptTicks();
        
        // Setup Newping Sensor
        void initUltra(uint8_t trigger_pin, uint8_t echo_pin, int max_cm_distance);
        // Get Newping ping in inches
        int ping_in();
        
    private:
        int _motorDirn_A;     // Motor A dirn pin
        int _motorSpeed_A;    // Motor A Speed pin
		int _motorDirn_B;     // Motor B dirn pin, need to manually reverse wiring
        int _motorSpeed_B;    // Motor B Speed pin
        int _servoPin;      // Servo Pin
        float _wheelCircumfrence;   // Wheel Circumfrence
        Servo _steeringServo;   // Front servo object
        volatile int _interruptTickCounter; // interup ticks counter
        float _compassHeading;	// Compass heading
        float* (*_headingFunc)();	// Pointer to compass heading function that returns int16_t pointer
		void _getHeading();	// Library function to call compass heading function and store the result
        
        // movement functions
        void _genMove(int dirn, int speed);
        
        // PID variables and methods
        void _logPID();
        float _getAdjustment();
        void _adjustServo(int baseAngle);
        int _getNextPositionPoint(int timeGap);
        int _pidPrevTime;
        float _PIDIntegral;
        float _PIDDerivative;
        int _prevPoint;
        int16_t _desiredDirn;
        float _pConst;
        float _iConst;
        float _dConst;
        
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
