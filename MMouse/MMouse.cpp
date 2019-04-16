
#include "MMouse.h"
#include "SoftwareSerial.h"
#include "Arduino.h"

#include <Button.h>
#include <Led.h>
#include <DRV8835MotorShield.h>
#include <NewPing.h>
#include <Melodies.h>

MMouse::MMouse(){
	runSpeed = 50;
	searchSpeed = 50;
	leftStage = 1;
	rightStage = 1;
	backStage = 1;
	leftCount = 0;
	rightCount = 0;
	lastLeftCount = -1;
	lastRightCount = -1;
	collision = false;
	backWall = false;
	collisionCount = 0;
	for(int i = 0; i < LAST_DISTANCE_SAMPLES; i++){
		lastLeftDistance[i] = -1;
		lastCenterDistance[i] = -1;
		lastRightDistance[i] = -1;
	}
	//motors.flipM1(true);
	//motors.flipM2(true);
}

void MMouse::initialize(int newSpeed){
	pinMode(encoder1_A, INPUT);
	pinMode(encoder2_A, INPUT);
	pinMode(encoder1_B, INPUT);
	pinMode(encoder2_B, INPUT);
	pinMode(motorA_1, OUTPUT);
	pinMode(motorA_2, OUTPUT);
	pinMode(motorB_1, OUTPUT);
	pinMode(motorB_2, OUTPUT);
	pinMode(buzzer, OUTPUT);
	pinMode(right_button, INPUT);
	pinMode(led, OUTPUT);
	pinMode(left_button, INPUT);
	pinMode(left_trigger, INPUT);
	pinMode(left_echo, INPUT);
	pinMode(right_trigger, INPUT);
	pinMode(right_echo, INPUT);
	pinMode(front_trigger, INPUT);
	pinMode(front_echo, INPUT);
	searchSpeed = newSpeed;
	runSpeed = newSpeed;
}

void MMouse::resetCollision(){
	collision = false;
	collisionCount = 0;
}

void MMouse::setRunSpeed(int newSpeed){
	runSpeed = newSpeed;
}

void MMouse::setSearchSpeed(int newSpeed){
	searchSpeed = newSpeed;
}

void MMouse::setDebug(bool option){
	debug = option;
}

void MMouse::setLeftCount(int newCount){
	leftCount = newCount;
}
void MMouse::setRightCount(int newCount){
	rightCount = newCount;
}

void MMouse::setLeftMotorSpeed(int newSpeed){
	leftMotorSpeed = newSpeed;
}

void MMouse::setRightMotorSpeed(int newSpeed){
	rightMotorSpeed = newSpeed;
}

void MMouse::setBackStage(int newStage){
	backStage = newStage;
}

void MMouse::setLeftStage(int newStage){
	leftStage = newStage;
}

void MMouse::setRightStage(int newStage){
	rightStage = newStage;
}

void MMouse::setLastRightCount(int newCount){
	lastRightCount = newCount;
}

void MMouse::setLastLeftCount(int newCount){
	lastLeftCount = newCount;
}

bool MMouse::checkCero(){ return (leftDistance == 0 || rightDistance == 0); }

void MMouse::incrementRunSpeed(SoftwareSerial out){
    music.playBeep();
    if (runSpeed + INCREMENT < MAX_SPEED) runSpeed = runSpeed + INCREMENT;
    else runSpeed = MAX_SPEED;
    if(debug) out.println(runSpeed);
}

void MMouse::incrementSearchSpeed(SoftwareSerial out){
    music.playBeep();
    if (runSpeed + INCREMENT < MAX_SPEED) searchSpeed = searchSpeed + INCREMENT;
    else searchSpeed = MAX_SPEED;
	runSpeed = searchSpeed;
    if(debug) out.println(searchSpeed);
}

void MMouse::waitForUser(){
	  motors.setSpeeds(0,0);
  for(int i = 0; i < 150; i++){
      leftDistance = leftDistanceSensor.ping_cm();
      delay(2);
      centerDistance = centerDistanceSensor.ping_cm();
      delay(2);
      rightDistance = rightDistanceSensor.ping_cm();
  }
}

void MMouse::goForward(int forwardSpeed){
	if((lastLeftCount == leftCount && lastRightCount == rightCount) || collision){
		collision = true;
		leftMotorSpeed = -MIN_SPEED;
		rightMotorSpeed = -MIN_SPEED;
		if(lastLeftCount == leftCount && lastRightCount == rightCount){
			aux_leftCount = leftCount;
			aux_rightCount = rightCount;
			collisionCount++;
		}
		if(leftCount < aux_leftCount - FORWARD_COLLISION_TICKS && rightCount < aux_rightCount - FORWARD_COLLISION_TICKS){
			collision = false;
			leftMotorSpeed = forwardSpeed;
			rightMotorSpeed = forwardSpeed;
		}
	}
	else{
		if(leftDistance == 0 && rightDistance == 0){
			  leftMotorSpeed = forwardSpeed;
			  rightMotorSpeed = forwardSpeed;
			}
		else if(leftDistance == 0 && rightDistance != 0){
			if(rightDistance == -1){
			  leftMotorSpeed = forwardSpeed;
			  rightMotorSpeed = forwardSpeed;
			}
			else if(rightDistance < 5){
			  leftMotorSpeed = forwardSpeed - RIGHT_DEV;
			  rightMotorSpeed = forwardSpeed + RIGHT_DEV;
			}
			else if(rightDistance < 7){
			  leftMotorSpeed = forwardSpeed;
			  rightMotorSpeed = forwardSpeed;
			}
			else{
			  leftMotorSpeed = forwardSpeed + LEFT_DEV;
			  rightMotorSpeed = forwardSpeed - LEFT_DEV; 
			}
		}
		else if(leftDistance != 0 && rightDistance == 0){
			 if(leftDistance == -1){
			  leftMotorSpeed = forwardSpeed;
			  rightMotorSpeed = forwardSpeed;
			 }
			 else if(leftDistance < 6){
			  leftMotorSpeed = forwardSpeed + LEFT_DEV;
			  rightMotorSpeed = forwardSpeed - LEFT_DEV;
			 }
			else if(leftDistance < 8){
			  leftMotorSpeed = forwardSpeed;
			  rightMotorSpeed = forwardSpeed;
			}
			else{
			  leftMotorSpeed = forwardSpeed - RIGHT_DEV;
			  rightMotorSpeed = forwardSpeed + RIGHT_DEV;
			}
		}
		else if(leftDistance != 0 && rightDistance != 0){
			if(leftDistance == -1) leftMotorSpeed = forwardSpeed;
			else if(leftDistance < 6) leftMotorSpeed = forwardSpeed + LEFT_DEV;
			else if(leftDistance < 8) leftMotorSpeed = forwardSpeed;
			else leftMotorSpeed = forwardSpeed - RIGHT_DEV;

			if(rightDistance == -1) rightMotorSpeed = forwardSpeed;
			else if(rightDistance < 5) rightMotorSpeed = forwardSpeed + RIGHT_DEV;
			else if(rightDistance < 7) rightMotorSpeed = forwardSpeed;
			else rightMotorSpeed = forwardSpeed - LEFT_DEV; 
		}
	}
	lastRightCount = rightCount;
	lastLeftCount = leftCount;
}

void MMouse::turnRight(){
	switch(rightStage){
		case 1:			
			if(lastLeftCount == leftCount || collision){
				collision = true;
				leftMotorSpeed = -MIN_SPEED;
				rightMotorSpeed = -MIN_SPEED;
				if(lastLeftCount == leftCount){
					aux_leftCount = leftCount;
					aux_rightCount = rightCount;
				}
				if(leftCount < aux_leftCount - TURN_COLLISION_TICKS && rightCount < aux_rightCount - TURN_COLLISION_TICKS){
					collision = false;
					leftCount = aux_leftCount;
					leftMotorSpeed = TURN_SPEED;
				}
			}
			else{
				leftMotorSpeed = TURN_SPEED;
			}
			if(leftCount >= TURN_RIGHT_TICKS){
				leftCount = 0;
				rightCount = 0;
				rightStage = 2;
			}
			lastLeftCount = leftCount;
			lastRightCount = rightCount;
		break;
		case 2:
			goForward(searchSpeed);
			if(collisionCount >= 2){
				rightStage = 1;
				collisionCount = 0;
			}
			if(leftCount >= TURN_POSFORWARD_TICKS && rightCount >= TURN_POSFORWARD_TICKS){
				rightStage = -1;
			}
		break;
	}
}

void MMouse::turnLeft(){
		switch(leftStage){
		case 1:
			if(lastRightCount == rightCount || collision){
				collision = true;
				leftMotorSpeed = -MIN_SPEED;
				rightMotorSpeed = -MIN_SPEED;
				if(lastRightCount == rightCount){
					aux_leftCount = leftCount;
					aux_rightCount = rightCount;
				}
				if(leftCount < aux_leftCount - TURN_COLLISION_TICKS && rightCount < aux_rightCount - TURN_COLLISION_TICKS){
					collision = false;
					rightCount = aux_rightCount;
					rightMotorSpeed = TURN_SPEED;
				}
			}
			else{
				rightMotorSpeed = TURN_SPEED;
			}
			if(rightCount >= TURN_LEFT_TICKS){
				leftCount = 0;
				rightCount = 0;
				leftStage = 2;
			}
			lastLeftCount = leftCount;
			lastRightCount = rightCount;
		break;
		case 2:
			goForward(searchSpeed);
			if(collisionCount >= 2){
				leftStage = 1;
				collisionCount = 0;
			}
			if(leftCount >= TURN_POSFORWARD_TICKS && rightCount >= TURN_POSFORWARD_TICKS){
				leftStage = -1;
			}
		break;
	}
}

void MMouse::turnAround(){
	switch(backStage){
		case 1:
		leftMotorSpeed = MIN_SPEED;
		rightMotorSpeed = MIN_SPEED;
		if(rightCount >= 10) backStage = 2;
		break;
		case 2:
		  if(leftDistance == 0) leftMotorSpeed = MIN_SPEED;
          else if(leftDistance < 6) leftMotorSpeed = MIN_SPEED + LEFT_DEV;
          else if(leftDistance < 8) leftMotorSpeed = MIN_SPEED;
          else leftMotorSpeed = MIN_SPEED - RIGHT_DEV;
    
          if(rightDistance == 0) rightMotorSpeed = MIN_SPEED;
          else if(rightDistance < 5) rightMotorSpeed = MIN_SPEED + RIGHT_DEV;
          else if(rightDistance < 7) rightMotorSpeed = MIN_SPEED;
          else rightMotorSpeed = MIN_SPEED - LEFT_DEV; 
		
          if(rightCount == lastRightCount){
            rightCount = 0;
            leftCount = 0;
            backStage = 3;
			backWall = true;
          }
		  else if(rightCount >= BACK_MAX_TICKS){
			  rightCount = 0;
			  leftCount = 0;
			  backStage = 3;
			  backWall = false;
		  }
          lastRightCount = rightCount;
		break;
		case 3:
		  rightMotorSpeed = -TURN_SPEED;
		  if(abs(rightCount) >= BACK_B_TICKS){
			rightCount = 0;
			leftCount = 0;
			backStage = 4;         
		  }
		break;
		case 4:
			leftMotorSpeed = TURN_SPEED;
			if(leftCount >= BACK_F_TICKS){
				leftCount = 0;
				rightCount = 0;
				backStage = 5;
			}
		break;
		case 5:
			goForward(searchSpeed);
			if(backWall){
				if(leftCount >= BACK_FORWARD_TICKS && rightCount >= BACK_FORWARD_TICKS){
					backStage = -1;
				}
			}
			else{
				if(leftCount >= BACK_MAX_FORWARD_TICKS && rightCount >= BACK_MAX_FORWARD_TICKS){
					backStage = -1;
				}
			}

		break;
	}
}

void MMouse::readDistance(){
	int aux_left[NUM_DISTANCE_SAMPLES], aux_right[NUM_DISTANCE_SAMPLES], aux_center[NUM_DISTANCE_SAMPLES];
    for(int i = 0; i < NUM_DISTANCE_SAMPLES; i++){
      aux_left[i] = leftDistanceSensor.ping_cm();
      delay(3);
      aux_center[i] = centerDistanceSensor.ping_cm();
      delay(3);
      aux_right[i] = rightDistanceSensor.ping_cm();
      delay(3);
    }
    leftDistance = choosePopularFromVector(aux_left);
    centerDistance = choosePopularFromVector(aux_center);
    rightDistance = choosePopularFromVector(aux_right);
	
	for(int i = 0; i < LAST_DISTANCE_SAMPLES-1; i++){
		lastLeftDistance[i] = lastLeftDistance[i+1];
		lastCenterDistance[i] = lastCenterDistance[i+1];
		lastRightDistance[i] = lastRightDistance[i+1];
	  }
	lastLeftDistance[LAST_DISTANCE_SAMPLES - 1] = leftDistance;
	lastCenterDistance[LAST_DISTANCE_SAMPLES-1] = centerDistance;
	lastRightDistance[LAST_DISTANCE_SAMPLES-1] = rightDistance;

}

void MMouse::writeMotorSpeed(){
	if(leftMotorSpeed == 0 && rightMotorSpeed == 0){
		motors.setSpeeds(0, 0);
	}
	else if(leftMotorSpeed == 0 && rightMotorSpeed != 0){
		motors.setSpeeds(0, rightMotorSpeed + RIGHT_MOTOR_FIT);
	}
	else if(leftMotorSpeed != 0 && rightMotorSpeed == 0){
		motors.setSpeeds(leftMotorSpeed + LEFT_MOTOR_FIT, 0);
	}
	else{
		motors.setSpeeds(leftMotorSpeed + LEFT_MOTOR_FIT, rightMotorSpeed + RIGHT_MOTOR_FIT);
	}
}

void MMouse::print(SoftwareSerial out){
	if(debug){
		out.println("DISTANCE SENSORS");
		out.print("Left: ");
		out.print(leftDistance);
		out.print("   Center: ");
		out.print(centerDistance);
		out.print("   Right: ");
		out.print(rightDistance);
		out.println();
		out.println();
		out.println("ENCODERS");
		out.print("Left: ");
		out.print(leftCount);
		out.print("   Right: ");
		out.println(rightCount);
		out.println();
		out.println();
		out.println("MOTORS");
		out.print("Left: ");
		out.print(leftMotorSpeed);
		out.print("   Right: ");
		out.println(rightMotorSpeed);
		for(int i = 0; i < 3; i++) out.println();	
	}
}

int MMouse::choosePopularFromVector(int v[]){
	if(v[0] == v[1]) return v[0];
	if(v[0] == v[2]) return v[0];
	if(v[1] == v[2]) return v[1];
	return -1;
}

int MMouse::choosePopular(int mode){
	switch(mode){
		case 0:
			if(lastLeftDistance[0] == lastLeftDistance[1]) return lastLeftDistance[0];
			if(lastLeftDistance[0] == lastLeftDistance[2]) return lastLeftDistance[0];
			if(lastLeftDistance[1] == lastLeftDistance[2]) return lastLeftDistance[1];
			return -1;
		break;
		case 1:
			if(lastCenterDistance[0] == lastCenterDistance[1]) return lastCenterDistance[0];
			if(lastCenterDistance[0] == lastCenterDistance[2]) return lastCenterDistance[0];
			if(lastCenterDistance[1] == lastCenterDistance[2]) return lastCenterDistance[1];
			return -1;
		break;
		case 2:
			if(lastRightDistance[0] == lastRightDistance[1]) return lastRightDistance[0];
			if(lastRightDistance[0] == lastRightDistance[2]) return lastRightDistance[0];
			if(lastRightDistance[1] == lastRightDistance[2]) return lastRightDistance[1];
			return -1;
		break;
	}
}