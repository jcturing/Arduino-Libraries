#ifndef Circuit_h
#define Circuit_h

#include "SoftwareSerial.h"
#include "Arduino.h"

const int DIM = 75;
const int STRAIGHT_CURVATURE = 25;
const int STRAIGHT_LENGHT = 3820;

struct Segment{
	long leftTicks, rightTicks;
	float speed;
};

class Circuit{
public:
	Circuit();
	
	int getTam(){return circuitTam;}
	long getLeft(int i){return circuit[i].leftTicks;}
	long getRight(int i){return circuit[i].rightTicks;}
	float getSpeed(int i){return circuit[i].speed;}
	void add(long i, long j);
	void print(SoftwareSerial out);
	
private:	
	Segment circuit[DIM];
	float cur_aux;
	int circuitTam;
};

#endif

