
#include "Cell.h"
#include "Arduino.h"

Cell::Cell(){
		up = false;
		left = false;
		value = MAX_VALUE;
}

Cell::Cell(byte newValue){
		up = false;
		left = false;
		value = newValue;
}

void Cell::setUp(bool wall){
	up = wall;
}

void Cell::setLeft(bool wall){
	left = wall;
}

void Cell::setValue(byte newValue){
	value = newValue;
}