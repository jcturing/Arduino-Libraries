
#include "Button.h"
#include "Arduino.h"

Button::Button(int pin){
		button = pin;
		buttonValue = false;
		buttonState = false;
}

bool Button::isPressed(){
	return digitalRead(button);
}

bool Button::wasPressed(){
	buttonValue = digitalRead(button);
	if(buttonValue) buttonState = true;
	else if(buttonState){
		buttonState = false;
		delay(500);
		return true;	
	}
	return false;
}
