#ifndef Cell_h
#define Cell_h

#include "Arduino.h"

const byte MAX_VALUE = 99;

class Cell{
public:
	Cell();
	Cell(byte newValue);
	
	
	//Getters
	bool getUp() const{ return up; }
	bool getLeft() const{ return left; }
	int  getValue() const{ return value;}
	
	//Setters
	void setUp(bool wall);
	void setLeft(bool wall);
	void setValue(byte newValue);
	
private:
	bool up, left; //True = pared, false = vacio
	byte value;
};

#endif

