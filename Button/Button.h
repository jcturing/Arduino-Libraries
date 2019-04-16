#ifndef Button_h
#define Button_h

#include "Arduino.h"

class Button{
public:
	Button(int pin);
	bool isPressed();
	bool wasPressed();
private:
	int button;
	bool buttonValue;
	bool buttonState;
};

#endif

