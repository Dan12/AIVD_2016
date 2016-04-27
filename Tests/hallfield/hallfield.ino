
const int signalPin = 9;

void setup(){
	Serial.begin(9600);
	pinMode(signalPin, INPUT);
}

void loop(){
	int value = analogRead(signalPin);
	Serial.println(value);
}
