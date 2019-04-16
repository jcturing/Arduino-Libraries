#ifndef Led_h
#define Led_h

#include "Arduino.h"

class Led{
public:
	Led(int pin);
	
	void turnOn();
	void turnOff();
	void switchState();
	
	bool isOn();
	bool isOff();
	bool getState();
	
private:
	int led;
	bool ledState;
};

#endif

