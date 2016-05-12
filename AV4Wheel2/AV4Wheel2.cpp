/*
 * AV4Wheel2-4 Wheel Drive library for AIVD
 * Daniel Weber 2016
**/

#include "Arduino.h"
#include "AV4Wheel2.h"

#define TICKS_PER_ROTATION 90   // Encoder value for the number of measured ticks per rotation
#define DEFAULT_SPEED 125	// default car speed
#define RAMP_DELAY 30	// milliseconds of ramp speed delay


AV4Wheel2::AV4Wheel2(){}

/*
AV4 Wheel Init function
md-motor dirn Pin
ms-motor speed Pin
sp-servo pin
wc-wheel circumfrence
*/
void AV4Wheel2::init(int mda, int msa, int mdb, int msb, int sp, float wc){
    _motorDirn_A = mda;
    _motorSpeed_A = msa;
    _motorDirn_B = mdb;
    _motorSpeed_B = msb;
    
    _servoPin = sp;
    
    _wheelCircumfrence = wc;
    
    pinMode(_motorDirn_A,OUTPUT);
    pinMode(_motorSpeed_A,OUTPUT);
	pinMode(_motorDirn_B,OUTPUT);
    pinMode(_motorSpeed_B,OUTPUT);
    
    _steeringServo.attach(_servoPin);
    
    resetInterruptTicks();
}

// Movement code

// general move
void AV4Wheel2::genMove(int dirn, int speed){
	// go in the specified direction
	digitalWrite(_motorDirn_A, dirn);
	digitalWrite(_motorDirn_B, dirn);

	// go at the default speed if no speed specified (-1)
	analogWrite(_motorSpeed_A, speed == -1 ? DEFAULT_SPEED : speed);
	analogWrite(_motorSpeed_B, speed == -1 ? DEFAULT_SPEED : speed);
}

void AV4Wheel2::setServo(int angle){
	_steeringServo.write(angle);
}

// ramp motion
void AV4Wheel2::rampMotion(int startSpeed, int finalSpeed, int delayTime, int steps, int dirn, int servoAngle){
	_steeringServo.write(servoAngle);
	
	// TODO: log PID when implemented with ticks
	
	// ramp up
	if(startSpeed < finalSpeed)
		for(int i = 0; i <= finalSpeed; i+=steps){
			genMove(dirn, i);
			delay(delayTime == -1 ? RAMP_DELAY : delayTime);
		}
	// ramp down
	else
		for(int i = startSpeed; i >= finalSpeed; i-=steps){
			genMove(dirn, i);
			delay(delayTime == -1 ? RAMP_DELAY : delayTime);
		}
}

// move a specific distance with the specified speed, dirn, and servo angle
void AV4Wheel2::moveDist(int dist, int dirn, int speed, int servoAngle){
	// reset tick counter
	resetInterruptTicks();
	
	// set servo angle
	//_steeringServo.write(servoAngle);
	Serial.println(servoAngle);
	Serial.println(_interruptTickCounter);
	Serial.println(dirn);
	Serial.println(speed);
	// calculate total ticks by dist/(dist/rotation)*ticks/rotation
	int totalTicks = dist/_wheelCircumfrence*TICKS_PER_ROTATION;
	Serial.println(totalTicks);
	
	// while we still have ticks to go, loop
	while(_interruptTickCounter < totalTicks){
		Serial.println(_interruptTickCounter);
		_logPID();
		genMove(dirn,speed);
		_adjustServo(servoAngle);
	}
}

// move until ultrasonic triggered
// move at the speed and in the dirn, if ultraTrigger == 1, move until ultraSonic is < ultraDist, visa versa
void AV4Wheel2::moveUltra(int speed, int dirn, int servoAngle, int ultraDist, int ultraTrigger){
	_steeringServo.write(servoAngle);
	genMove(dirn, speed);
	
	int curDist = ping_in();
	
	// trigger when object closer than ultraDist
	if(ultraTrigger == 1){
		while(curDist == -1 || curDist > ultraDist){
			// minimum delay between pings
			delay(30);
			_logPID();
			_adjustServo(servoAngle);
			curDist = ping_in();
		}
	}
	else{
		while(curDist == -1 || curDist < ultraDist){
			// minimum delay between pings
			delay(30);
			_logPID();
			_adjustServo(servoAngle);
			curDist = ping_in();
		}
	}
}

// set the servo angle and move the car until the compass is facing the desired heading
// give Clock Wise and Counter Clock Wise servo angles, function will figure out best one to use
	// note that because of slow servo response time, the car might drift a little after coming out of the turn
	// however, if the PID starts to log immediately after, it should correct itself to the center of the track
void AV4Wheel2::changeHeading(int speed, int dirn, int servoAngleCW, int servoAngleCCW, float gotoHeading){
	
	// set the heading variable
	_getHeading();
	
	// distance between the two angles
	float distance = abs(_compassHeading-gotoHeading);
	if(distance > 180)
		distance -= 180;
		
	int servoAngle = 90;
	// turning CCW	
	if((_compassHeading-gotoHeading > 0 && _compassHeading-gotoHeading < 180) || gotoHeading-_compassHeading > 180)
		servoAngle = servoAngleCCW;
	// turn CW
	else
		servoAngle = servoAngleCW;
	
	_steeringServo.write(servoAngle);
	float prevAngle = _compassHeading;
	while(distance > 0){
		genMove(dirn,speed);
		_getHeading();
		float diff = abs(_compassHeading-prevAngle);
		distance -= diff > 180 ? 360-diff : diff;
		
		_logPID();
		_adjustServo(servoAngle);
	}
	
}

void AV4Wheel2::stopCar(){
	genMove(LOW,0);
}

// PID stuff
// set the PID variables
void AV4Wheel2::initPID(float p, float i, float d, float s, float min, float max){
	_PIDDerivative = 0;
	_PIDIntegral = 0;
	
	_pConst = p;
	_iConst = i;
	_dConst = d;
	_pidTimeScale = s;
	
	_pidRunning = false;
	
	_uMin = min;
	_uMax = max;
}

void AV4Wheel2::resetPID(){
	_PIDDerivative = 0;
	_PIDIntegral = 0;
}

void AV4Wheel2::stopPID(){			
	_pidRunning = false;
}

// start PID logging and reset PID variables
void AV4Wheel2::startPID(boolean t){
	_usingTicks = t;
	if(_usingTicks)
		_pidPrevTime = _interruptTickCounter;
	else
		_pidPrevTime = millis();
	
	_pidRunning = true;
}

void AV4Wheel2::viewPID(){
	Serial.print("Integral: ");
	Serial.print(_PIDIntegral);
	Serial.print("   Adjustment: ");
	Serial.print(_getAdjustment());	
}

float AV4Wheel2::getPIDIntegral(){
	return _PIDIntegral;
}

// record PID data for current time
// TODO: record forward distance loss using pythagorean theorem
void AV4Wheel2::_logPID(){
	int tempTime = 0;
	if(_usingTicks)
		tempTime = _interruptTickCounter;
	else
		tempTime = millis();
	
	float timeGap = (_pidPrevTime - tempTime)*_pidTimeScale;
	float nextPoint = _getNextPositionPoint(timeGap);
	
	// rise over run
	_PIDDerivative = (nextPoint-_prevPoint)/timeGap;
	// trapazoidal integral
	_PIDIntegral += timeGap*(_prevPoint + 0.5f*(nextPoint-_prevPoint));
	
	
	_prevPoint = nextPoint;
	_pidPrevTime = tempTime;
}

// get the next y position for the PID controller given the time gap between the previous measurement
float AV4Wheel2::_getNextPositionPoint(float timeGap){
		// need prev position point, new position point is prev_position+(heading-desired_heading)*time_gap
		return _prevPoint+(_compassHeading-_desiredDirn)*timeGap;
}

// set the desired PID heading, PID should keep car going in straing line
void AV4Wheel2::setPIDHeading(float dirn){
	_desiredDirn = dirn;
}

// get servo adjustment from PID
// Might have to take the integral of compass heading w.r.t. time to get actual PID integral
// position is the integral of compass heading because error grows at constant rate if heading is off
float AV4Wheel2::_getAdjustment(){
	return _prevPoint*_pConst + _PIDIntegral*_iConst + _PIDDerivative*_dConst;
}

// adjust servo based on base angle for serve
void AV4Wheel2::_adjustServo(int baseAngle){
	_steeringServo.write(baseAngle - (_pidRunning ? constrain(_getAdjustment(), _uMin, _uMax) : 0));
}

// compass code
void AV4Wheel2::initCompass(float* (*func)()){
	// create array by getting result of the function call
	// turn pointer of result to actual value and use it to initialze the array
	float temp = *(*func)();
	Serial.println(temp);
	// store the heading function
	_headingFunc = func;
}

// Get the heading from the heading function
void AV4Wheel2::_getHeading(){
	_compassHeading = *(*_headingFunc)();
}

// Interrupt code

// Interupt function to be linked in the main program
void AV4Wheel2::interrupEncoderFunc(){
    _interruptTickCounter++;
}

// reset ticks
void AV4Wheel2::resetInterruptTicks(){
	_interruptTickCounter = 0;
}

// get the Interupt ticks
int AV4Wheel2::getInterrupTicks(){
    return _interruptTickCounter;
}

float AV4Wheel2::getInterruptDist(){
	// ticks   /   ticks per rotation   * distance per rotation
	return 1.0*_interruptTickCounter/TICKS_PER_ROTATION*_wheelCircumfrence;
}

// Test the encoder interrupt function
void AV4Wheel2::testInterrupt(){
    resetInterruptTicks();
    while(_interruptTickCounter < 4*TICKS_PER_ROTATION){
        Serial.print(_interruptTickCounter);
        Serial.println(" Ticks Done");
    }
}

// NewPing Functions and variables
// Shoudln't need to changed these values unless you have a specific need to do so.
#define MAX_SENSOR_DISTANCE 500 // Maximum sensor distance can be as high as 500cm, no reason to wait for ping longer than sound takes to travel this distance and back.
#define US_ROUNDTRIP_IN 146     // Microseconds (uS) it takes sound to travel round-trip 1 inch (2 inches total), uses integer to save compiled code space.
#define US_ROUNDTRIP_CM 57      // Microseconds (uS) it takes sound to travel round-trip 1cm (2cm total), uses integer to save compiled code space.
#define DISABLE_ONE_PIN false   // Set to "true" to save up to 26 bytes of compiled code space if you're not using one pin sensor connections.

// Probably shoudln't change these values unless you really know what you're doing.
#define NO_ECHO -1               // Value returned if there's no ping echo within the specified MAX_SENSOR_DISTANCE or max_cm_distance.
#define MAX_SENSOR_DELAY 18000  // Maximum uS it takes for sensor to start the ping (SRF06 is the highest measured, just under 18ms).
// #define ECHO_TIMER_FREQ 24      // Frequency to check for a ping echo (every 24uS is about 0.4cm accuracy).
// #define PING_MEDIAN_DELAY 29    // Millisecond delay between pings in the ping_median method.

// Init method, set current ultrasonic trigger and echo pin
void AV4Wheel2::initUltra(uint8_t trigger_pin, uint8_t echo_pin, int max_cm_distance) {
    _triggerBit = digitalPinToBitMask(trigger_pin); // Get the port register bitmask for the trigger pin.
    _echoBit = digitalPinToBitMask(echo_pin);       // Get the port register bitmask for the echo pin.

    _triggerOutput = portOutputRegister(digitalPinToPort(trigger_pin)); // Get the output port register for the trigger pin.
    _echoInput = portInputRegister(digitalPinToPort(echo_pin));         // Get the input port register for the echo pin.

    _triggerMode = (uint8_t *) portModeRegister(digitalPinToPort(trigger_pin)); // Get the port mode register for the trigger pin.

    _maxEchoTime = min(max_cm_distance, MAX_SENSOR_DISTANCE) * US_ROUNDTRIP_CM + (US_ROUNDTRIP_CM / 2); // Calculate the maximum distance in uS.

    #if DISABLE_ONE_PIN == true
        *_triggerMode |= _triggerBit; // Set trigger pin to output.
    #endif
}

// Ping in inches
int AV4Wheel2::ping_in(){
    if (!_ping_trigger()) return NO_ECHO;                // Trigger a ping, if it returns false, return NO_ECHO to the calling function.
    while (*_echoInput & _echoBit)                      // Wait for the ping echo.
        if (micros() > _max_time) return NO_ECHO;       // Stop the loop and return NO_ECHO (false) if we're beyond the set maximum distance.
    int uS = (micros() - (_max_time - _maxEchoTime) - 5); // Calculate ping time, 5uS of overhead.
    return uS / US_ROUNDTRIP_IN; 
}

boolean AV4Wheel2::_ping_trigger() {
    #if DISABLE_ONE_PIN != true
        *_triggerMode |= _triggerBit;    // Set trigger pin to output.
    #endif
        *_triggerOutput &= ~_triggerBit; // Set the trigger pin low, should already be low, but this will make sure it is.
        delayMicroseconds(4);            // Wait for pin to go low, testing shows it needs 4uS to work every time.
        *_triggerOutput |= _triggerBit;  // Set trigger pin high, this tells the sensor to send out a ping.
        delayMicroseconds(10);           // Wait long enough for the sensor to realize the trigger pin is high. Sensor specs say to wait 10uS.
        *_triggerOutput &= ~_triggerBit; // Set trigger pin back to low.
    #if DISABLE_ONE_PIN != true
        *_triggerMode &= ~_triggerBit;   // Set trigger pin to input (when using one Arduino pin this is technically setting the echo pin to input as both are tied to the same Arduino pin).
    #endif

    _max_time =  micros() + MAX_SENSOR_DELAY;                  // Set a timeout for the ping to trigger.
    while (*_echoInput & _echoBit && micros() <= _max_time) {} // Wait for echo pin to clear.
    while (!(*_echoInput & _echoBit))                          // Wait for ping to start.
        if (micros() > _max_time) return false;                // Something went wrong, abort.

    _max_time = micros() + _maxEchoTime; // Ping started, set the timeout.
    return true;                         // Ping started successfully.
}
