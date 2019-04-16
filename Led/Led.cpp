
#include "Led.h"
#include "Arduino.h"

Led::Led(int pin){
		led = pin;
		ledState = false;
}

void Led::turnOn(){
	ledState = true;
	digitalWrite(led, ledState);
}

void Led::turnOff(){
	ledState = false;
	digitalWrite(led, ledState);
}

void Led::switchState(){
	if(ledState) ledState = false;
	else ledState = true;
	digitalWrite(led, ledState);
}

bool Led::isOn(){
	return ledState == true;
}

bool Led::isOff(){
	return ledState == false;
}

bool Led::getState(){
	return ledState;
}
