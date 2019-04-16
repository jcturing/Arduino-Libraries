
#include "Circuit.h"
#include "SoftwareSerial.h"
#include "Arduino.h"

Circuit::Circuit(){
	circuitTam = 0;
}

void Circuit::add(long i, long j){
	circuit[circuitTam].leftTicks = i;
	circuit[circuitTam].rightTicks = j;
	cur_aux = (100*(i-j))/(i+j);
	if(abs(cur_aux) <= STRAIGHT_CURVATURE && (i+j)/2 >= STRAIGHT_LENGHT){
		circuit[circuitTam].speed = 2-(0.01*abs(cur_aux));
	}
	else circuit[circuitTam].speed = 1;
	circuitTam++;
}

void Circuit::print(SoftwareSerial out){
	out.println("Circuit: ");
	for(int i = 0; i < circuitTam; i++){
		out.print(circuit[i].leftTicks);
		out.print("\t");
		out.print(circuit[i].rightTicks);
		out.print("\t");
		out.println(circuit[i].speed);
	}
	for(int i = 0; i < 4; i++) out.println();
}