#ifndef MMouse_h
#define MMouse_h

#include "Cell.h"
#include "SoftwareSerial.h"
#include "Arduino.h"

#include <Button.h>
#include <Led.h>
#include <DRV8835MotorShield.h>
#include <NewPing.h>
#include <Melodies.h>


//PINES & CONSTANTES
const int RX = 0;
const int TX = 1;
const int encoder1_A = 2;
const int encoder2_A = 3;
const int encoder1_B = 4;
const int encoder2_B = 5;
const int left_button = 6;
const int motorA_1 = 7;
const int motorA_2 = 8;
const int motorB_1 = 9;
const int motorB_2 = 10;
const int right_button = 11;
const int led = 12;
const int buzzer = 13;
const int left_trigger = A0;
const int left_echo = A1;
const int right_trigger = A3;
const int right_echo = A2;
const int front_trigger = A5;
const int front_echo = A4;


const int MAX_SPEED = 400;
const int MIN_SPEED = 30;
const int INCREMENT = 10;
const int TURN_SPEED = 75;

const int MAX_SIDE_DISTANCE = 10;
const int MAX_FRONT_DISTANCE = 18;
const int NUM_DISTANCE_SAMPLES = 3;
const int BACK_FRONT_DISTANCE = 3;
const int MIN_FRONT_DISTANCE = 5;

const int RIGHT_DEV = 10;
const int LEFT_DEV = 20;

//SEARCH CALIBRATION
const int FORWARD_TICKS = 687; //Importante: All ticks should be less than forward ticks
const int FORWARD_COLLISION_TICKS = 100;

const int TURN_CHECK = 450;
const int TURN_RIGHT_TICKS = 505;
const int TURN_LEFT_TICKS = 505;
const int TURN_PREFORWARD_TICKS = 180;
const int TURN_POSFORWARD_TICKS = 260;
const int TURN_COLLISION_TICKS = 50;

const int BACK_MAX_TICKS = 325;
const int BACK_B_TICKS = 600;
const int BACK_F_TICKS = 490;
const int BACK_FORWARD_TICKS = 40;
const int BACK_MAX_FORWARD_TICKS = 280;

const int LEFT_MOTOR_FIT = 5;
const int RIGHT_MOTOR_FIT = 0;

const int LAST_DISTANCE_SAMPLES = 3;

class MMouse{
public:
	MMouse();
	
	int getRunSpeed() const{return runSpeed;}
	int getSearchSpeed() const{return searchSpeed;}
	int getLeftCount() const{return leftCount;}
	int getRightCount() const{return rightCount;}
	int getLeftDistance() const{return leftDistance;}
	int getCenterDistance() const{return centerDistance;}
	int getRightDistance() const{return rightDistance;}
	int getLeftStage() const{return leftStage;}
	int getRightStage() const{return rightStage;}
	int getBackStage() const{return backStage;}
	
	void setRunSpeed(int newSpeed);
	void setSearchSpeed(int newSpeed);
	void setDebug(bool option);
	void setLeftCount(int newCount);
	void setRightCount(int newCount);
	void setLeftMotorSpeed(int newSpeed);
	void setRightMotorSpeed(int newSpeed);
	void setBackStage(int newStage);
	void setLeftStage(int newStage);
	void setRightStage(int newStage);
	void setLastRightCount(int newCount);
	void setLastLeftCount(int newCount);
	
	void resetCollision();
	void initialize(int newSpeed);
	bool checkCero();
	void incrementRunSpeed(SoftwareSerial out);
	void incrementSearchSpeed(SoftwareSerial out);
	void waitForUser();
	void goForward(int forwardSpeed);
	void turnRight();
	void turnLeft();
	void turnAround();
	void readDistance();
	void writeMotorSpeed();
	void print(SoftwareSerial out);
	int choosePopularFromVector(int v[]);
	int choosePopular(int mode);

	DRV8835MotorShield motors; //M1 -> LEFT - M2 -> RIGHT

	NewPing leftDistanceSensor = NewPing(left_trigger, left_echo, MAX_SIDE_DISTANCE);
	NewPing centerDistanceSensor = NewPing(front_trigger, front_echo, MAX_FRONT_DISTANCE);
	NewPing rightDistanceSensor = NewPing(right_trigger, right_echo, MAX_SIDE_DISTANCE);

	Melodies music = Melodies(buzzer);

	Button leftButton = Button(left_button);
	Button rightButton = Button(right_button);

	Led rightLed = Led(led);
	
private:
	int searchSpeed, runSpeed;
	int leftStage, rightStage, backStage;
	volatile long leftCount;
	volatile long rightCount;
	long lastRightCount, lastLeftCount, aux_leftCount, aux_rightCount;
	int leftDistance, centerDistance, rightDistance;
	int leftMotorSpeed, rightMotorSpeed;
	bool debug, collision, backWall;
	int collisionCount;
	int lastLeftDistance[LAST_DISTANCE_SAMPLES];
	int lastCenterDistance[LAST_DISTANCE_SAMPLES];
	int lastRightDistance[LAST_DISTANCE_SAMPLES];
};

#endif

