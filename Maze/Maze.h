#ifndef Maze_h
#define Maze_h

#include "Cell.h"
#include "SoftwareSerial.h"
#include "Arduino.h"

const byte DIM = 8;
const int MAX_COMMAND = 40;
const int MAX_STACK = 36;

struct Pair{
	byte x, y;
	Pair(){}
	Pair(byte i, byte j){
		x = i;
		y = j;
	}
};

struct Command {
  char type;
  byte num = 0;
  Command(){}
  Command(char t, byte n){
	  type = t;
	  num = n;
  }
};

class Maze{
public:
	Maze();
		
	void setDraw(bool option);
	void setDebug(bool option);
	void setShowValues(bool option);
	void setX(int newX);
	void setY(int newY);
	void setOri(char newOri);
	void setChoice(char newChoice);
	
	int getX() const{return x;}
	int getY() const{return y;}
	char getOri() const{return ori;}
	char getChoice() const{return choice;}
	bool getDraw() const{return draw;}
	Command getCommand(int i) const{return commandList[i];}
	int getCommandListTam() const{return commandListTam;}
	
	void clear();
	void resetStart();
	void initialize();
	//void cybertech();
	//void bigCybertech();
	//void dimThree();
	void flood(bool center);
	void route(SoftwareSerial out);
	void updatePosition();
	void updateMap(bool left, bool center, bool right);
	void updateOri(); //F-R-L-B
	void makeChoice();
	bool isGoal();
	void print(SoftwareSerial out);
	void addCommand(Command com);
	
private:
	bool found(byte w, byte z, Pair v[], int tam);
	
	Cell maze[DIM][DIM];
	
	Pair currentLevel[MAX_STACK];
	int currentLevelTam;
	Pair nextLevel[MAX_STACK];
	int nextLevelTam;
	Command commandList[MAX_COMMAND];
	int commandListTam;
	bool draw, debug, showValues;
	int x, y;
	char ori; //N-E-S-W
	char choice;
};

#endif

