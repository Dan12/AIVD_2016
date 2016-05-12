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
        void init(int mda, int msa, int mdb, int msb, int sp, float wc); // init core variables and pins
        void rampMotion(int startSpeed, int finalSpeed, int delayTime, int steps, int dirn, int servoAngle);	// ramp from start to final speed (use default delay if delay is -1)
        void moveDist(int dist, int dirn, int speed, int servoAngle);	// move a certain distance at a speed and servo angle
        void moveUltra(int speed, int dirn, int servoAngle, int ultraDist, int ultraTrigger);	// move until ultrasonic threshold reached
        void changeHeading(int speed, int dirn, int servoAngleCW, int servoAngleCCW, float gotoHeading);	// move until turned to given compass angle
        void stopCar();	// stop all car motion (only use after ramped down to ensure a full stop)
        
        // movement functions
        void genMove(int dirn, int speed);	// limited generic move
        void setServo(int angle);
        
        //Public PID functions
        void initPID(float p, float i, float d, float s, float min, float max); // init PID constants and scale
        void startPID(boolean t);	// start pid recording
        void stopPID();	// stop pid adjustments
        void resetPID();	// reset pid variables
        void viewPID();	// view PID variables
        float getPIDIntegral();
        
        // Compass functions
		void initCompass(float* (*func)());	// give pointer to compass heading function
        void setPIDHeading(float dirn);	// set PID desired heading
        
        // Interupt functions
        void interrupEncoderFunc();	// interupt function to be called in main program
        int getInterrupTicks();	// method to get interupt ticks
        float getInterruptDist();	// convert ticks to distance
        void testInterrupt();	// run interrupt tests
        void resetInterruptTicks();	// reset tick counter
        
        // Ultrasonic function
        void initUltra(uint8_t trigger_pin, uint8_t echo_pin, int max_cm_distance);	// Setup Newping Sensor
        int ping_in();	// Get Newping ping distance in inches
        
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
        
        // PID variables and methods
        void _logPID();	// log PID variables at the instance called
        float _getAdjustment();	// get servo adjustment based on PID
        void _adjustServo(int baseAngle);	// set servo angle based on PID adjustments
        float _getNextPositionPoint(float timeGap);	// get next position because heading is derivative of position
        int _pidPrevTime;	// store previous time to get time gap
        float _PIDIntegral;	// track integral
        float _PIDDerivative;	// track derivative
        float _prevPoint;	// store the previous position point
        int16_t _desiredDirn;	// the desired direction
        float _pConst;	// P Constant
        float _iConst;	// I Constant
        float _dConst;	// D constant
        float _pidTimeScale;
        float _uMin;
        float _uMax;
        boolean _pidRunning;	// Only make servo adjustments if this is true
        boolean _usingTicks;
        
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
