
#include "Maze.h"
#include "Cell.h"
#include "SoftwareSerial.h"
#include "Arduino.h"

Maze::Maze(){
	x = DIM-1;
	y = 0;
	ori = 'N';
	currentLevelTam = 0;
	nextLevelTam = 0;
	commandListTam = 0;
	//choice = 'F';
	clear();
}

void Maze::setDraw(bool option){
	draw = option;
}

void Maze::setDebug(bool option){
	debug = option;
}

void Maze::setShowValues(bool option){
	showValues = option;
}

void Maze::clear(){
	Cell newCell;
	  for(int i = 0; i < DIM; i++){
		for(int j = 0; j < DIM; j++){
		  maze[i][j] = newCell;
		}
	  }
}

void Maze::resetStart(){
	x = DIM-1;
	y = 0;
	ori = 'N';
}

void Maze::setX(int newX){
	x = newX;
}

void Maze::setY(int newY){
	y = newY;
}

void Maze::setOri(char newOri){
	ori = newOri;
}

void Maze::setChoice(char newChoice){
	choice =  newChoice;
}

void Maze::initialize(){
	  for(int i = 0; i < DIM; i++){
		maze[0][i].setUp(true);
		maze[i][0].setLeft(true);
	  }
	  maze[DIM-1][1].setLeft(true);
}

void Maze::flood(bool target){
	bool done = false;
	int level = 0;
	currentLevelTam = 0;
	nextLevelTam = 0;
	
	for(int i = 0; i < DIM; i++){
		for(int j = 0; j < DIM; j++){
			maze[i][j].setValue(MAX_VALUE);
		}
	}
	
	if(target){ //Write TARGET CELL!
		/*
		currentLevel[currentLevelTam] = Pair(0, 3);
		currentLevelTam++;
		*/
		
		currentLevel[currentLevelTam] = Pair((DIM/2)-1, (DIM/2)-1);
		currentLevelTam++;
		currentLevel[currentLevelTam] = Pair((DIM/2)-1, DIM/2);
		currentLevelTam++;
		currentLevel[currentLevelTam] = Pair(DIM/2, (DIM/2)-1);
		currentLevelTam++;
		currentLevel[currentLevelTam] = Pair(DIM/2, DIM/2);
		currentLevelTam++;
		
	}
	else{
		currentLevel[currentLevelTam] = Pair(DIM-1, 0);
		currentLevelTam++;
	}
	
	while(!done){
		while(!currentLevelTam == 0){
			Pair p = currentLevel[currentLevelTam-1];		
			currentLevelTam--;
			if(maze[p.x][p.y].getValue() == MAX_VALUE){
				maze[p.x][p.y].setValue(level);
				if (!maze[p.x][p.y].getUp() && maze[p.x - 1][p.y].getValue() == MAX_VALUE && !found(p.x-1, p.y, nextLevel, nextLevelTam)) {
					nextLevel[nextLevelTam] = Pair(p.x - 1, p.y);
					nextLevelTam++;
				}
				if (p.y < DIM - 1) {
					if (!maze[p.x][p.y + 1].getLeft() && maze[p.x][p.y + 1].getValue() == MAX_VALUE && !found(p.x, p.y + 1, nextLevel, nextLevelTam)) {
						nextLevel[nextLevelTam] = Pair(p.x, p.y + 1);
						nextLevelTam++;
					}
				}
				if (p.x < DIM - 1) {
					if (!maze[p.x + 1][p.y].getUp() && maze[p.x + 1][p.y].getValue() == MAX_VALUE && !found(p.x + 1, p.y, nextLevel, nextLevelTam)) {
						nextLevel[nextLevelTam] = Pair(p.x + 1, p.y);
						nextLevelTam++;
					}
				}
				if (!maze[p.x][p.y].getLeft() && maze[p.x][p.y - 1].getValue() == MAX_VALUE && !found(p.x, p.y - 1, nextLevel, nextLevelTam)) {
					nextLevel[nextLevelTam] = Pair(p.x, p.y - 1);
					nextLevelTam++;
				}	
			}

		}
		if(!nextLevelTam == 0){
			level++;
			for(int i = 0; i < nextLevelTam; i++){
				currentLevel[i] = nextLevel[i];
			}
			currentLevelTam = nextLevelTam;
			nextLevelTam = 0;
		}
		else done = true;
	}
}

void Maze::route(SoftwareSerial out){
	flood(true);
	bool anterior = false;
	int auxValue = MAX_VALUE+1;
	char nextDir;
	int runLenght = 0;
	int i = DIM-1, j = 0;
	char auxOri = 'N';
	commandListTam = 0;
	while(maze[i][j].getValue() != 0){
		Command auxCommand;
		if(!maze[i][j].getUp()){
			if(maze[i-1][j].getValue() < auxValue){
				auxValue = maze[i-1][j].getValue();
				nextDir = 'U';
			}
		}
		if((j < DIM - 1 ) && !maze[i][j+1].getLeft()){
			if(maze[i][j+1].getValue() < auxValue){
				auxValue = maze[i][j+1].getValue();
				nextDir = 'R';
			}
		}
		if((i < DIM - 1) && !maze[i+1][j].getUp()){
			if(maze[i+1][j].getValue() < auxValue){
				auxValue = maze[i+1][j].getValue();
				nextDir = 'D';
			}
		}
		if(!maze[i][j].getLeft()){
			if(maze[i][j-1].getValue() < auxValue){
				auxValue = maze[i][j-1].getValue();
				nextDir = 'L';
			}
		}

		switch(nextDir){
			case 'U':
				if(auxOri == 'N'){
					runLenght++;
				}
				else{
					if(runLenght > 0){
						auxCommand.type = 'F';
						auxCommand.num = runLenght;
						commandList[commandListTam] = auxCommand;
						commandListTam++;
						runLenght = 0;
					}
					if(auxOri == 'E') auxCommand.type = 'L';
					else if(auxOri == 'W') auxCommand.type = 'R';
					auxCommand.num = 1;
					commandList[commandListTam] = auxCommand;
					commandListTam++;
				}				
				i = i - 1;
			break;
			case 'R':
				if(auxOri == 'E'){
					runLenght++;
				} 
				else{
					if(runLenght > 0){
						auxCommand.type = 'F';
						auxCommand.num = runLenght;
						commandList[commandListTam] = auxCommand;
						commandListTam++;
						runLenght = 0;
					}
					if(auxOri == 'S') auxCommand.type = 'L';
					else if(auxOri == 'N') auxCommand.type = 'R';
					auxCommand.num = 1;
					commandList[commandListTam] = auxCommand;
					commandListTam++;
				}			
				j = j + 1;
			break;
			case 'D':
				if(auxOri == 'S'){
					runLenght++;
				} 
				else{
					if(runLenght > 0){
						auxCommand.type = 'F';
						auxCommand.num = runLenght;
						commandList[commandListTam] = auxCommand;
						commandListTam++;
						runLenght = 0;
					}
					if(auxOri == 'W') auxCommand.type = 'L';
					else if(auxOri == 'E') auxCommand.type = 'R';
					auxCommand.num = 1;
					commandList[commandListTam] = auxCommand;
					commandListTam++;
				}			
				i = i + 1;
			break;
			case 'L':
				if(auxOri == 'W'){
					runLenght++;
				} 
				else{
					if(runLenght > 0){
						auxCommand.type = 'F';
						auxCommand.num = runLenght;
						commandList[commandListTam] = auxCommand;
						commandListTam++;
						runLenght = 0;
					}
					if(auxOri == 'N') auxCommand.type = 'L';
					else if(auxOri == 'S') auxCommand.type = 'R';
					auxCommand.num = 1;
					commandList[commandListTam] = auxCommand;
					commandListTam++;
				}			
				j = j - 1;
			break;
		}
		if(nextDir == 'U') auxOri = 'N';
		else if(nextDir == 'R') auxOri = 'E';
		else if(nextDir == 'D') auxOri = 'S';
		else if(nextDir == 'L') auxOri = 'W';
		auxValue = MAX_VALUE;
	}
	if(runLenght > 0){
		Command auxCommand;
		auxCommand.type = 'F';
		auxCommand.num = runLenght;
		commandList[commandListTam] = auxCommand;
		commandListTam++;
	}
	
	if(debug){
		for(int i = 0; i < commandListTam; i++){
        out.print(commandList[i].type);
        out.print("   ");
        out.println(commandList[i].num);
       }
	   out.println(commandListTam);
	}
}

void Maze::updatePosition(){
	if(ori == 'N') x = x-1;
	else if(ori == 'E') y = y+1;
	else if(ori == 'S') x = x+1;
	else if(ori == 'W') y = y-1;
}

void Maze::updateMap(bool left, bool center, bool right){
	switch(ori){
	case 'N':
		if(left) maze[x][y].setLeft(true);
		if(center) maze[x][y].setUp(true);
		if((y < DIM-1) && right) maze[x][y+1].setLeft(true);
	break;
	case 'E':
		if(left) maze[x][y].setUp(true);
		if((y < DIM-1) && center) maze[x][y+1].setLeft(true);
		if((x < DIM-1) && right) maze[x+1][y].setUp(true);
	break;
	case 'S':
		if((y < DIM-1) && left) maze[x][y+1].setLeft(true);
		if((x < DIM-1) && center) maze[x+1][y].setUp(true);
		if(right) maze[x][y].setLeft(true);
	break;
	case 'W':
		if((x < DIM-1) && left) maze[x+1][y].setUp(true);
		if(center) maze[x][y].setLeft(true);
		if(right) maze[x][y].setUp(true);
	break;
	}
}

void Maze::updateOri(){
	switch(ori){
	case 'N':
		if(choice == 'R') ori = 'E';
		else if(choice == 'L') ori = 'W';
		else if(choice == 'B') ori = 'S';
	break;
	case 'E':
		if(choice == 'R') ori = 'S';
		else if(choice == 'L') ori = 'N';
		else if(choice == 'B') ori = 'W';
	break;
	case 'S':
		if(choice == 'R') ori = 'W';
		else if(choice == 'L') ori = 'E';
		else if(choice == 'B') ori = 'N';
	break;
	case 'W':
		if(choice == 'R') ori = 'N';
		else if(choice == 'L') ori = 'S';
		else if(choice == 'B') ori = 'E';
	break;
	}
}

bool Maze::isGoal(){
	return (maze[x][y].getValue() == 0);
}

void Maze::addCommand(Command com){
	commandList[commandListTam] = com;
	commandListTam++;
}

void Maze::print(SoftwareSerial out){
	  if(debug && draw){
		String aux;
		for(int i = 0; i < DIM; i++){
		  out.print("+");
		   for(int j = 0; j < DIM; j++){
			  if(maze[i][j].getUp()) aux = "----";
			  else aux = "    ";
			  out.print(aux);
			  out.print("+");
		   }
		  out.println();
		  
		   for(int j = 0; j < DIM; j++){
			  if(maze[i][j].getLeft()) aux = "|";
			  else aux = " ";
			  out.print(aux);
			  if(getX() == i && getY() == j) out.print(" @@ ");
			  else{
				if(showValues){
					if(maze[i][j].getValue() < -9){
						out.print(maze[i][j].getValue());
					}
					else if(maze[i][j].getValue() < 0){
						out.print(" ");
						out.print(maze[i][j].getValue());
					}
					else if(maze[i][j].getValue() == 0){
						out.print("  ");
						out.print(maze[i][j].getValue());
					}
					else if(maze[i][j].getValue() > 9){
						out.print(" ");
						out.print(maze[i][j].getValue());
					}
					else{
						out.print("  ");
						out.print(maze[i][j].getValue());
					}
					out.print(" ");
				}
				else{
					out.print("    ");
				}
			  }
		   }
		   out.print("|");
		   
		  out.println();
		 }  
		 
		out.print("+");
		for(int j = 0; j < DIM; j++){
		out.print("----");
		out.print("+");
		}
		
		out.println();
		out.print("X = ");
		out.print(x);
		out.print("     Y = ");
		out.print(y);
		out.print("     Orientation = ");
		out.print(ori);
		
		out.println();
		out.println();
		out.print("Choice = ");
		out.println(choice);
		
		for(int i = 0; i < 4; i++) out.println();
		draw = false;
	  }
}

bool Maze::found(byte w, byte z, Pair v[], int tam) {
	for (int i = 0; i < tam; i++) {
		if (w == v[i].x && z == v[i].y) return true;
	}
	return false;
}

void Maze::makeChoice(){
	int auxValue = MAX_VALUE+1;
	char nextDir;
	
	switch(ori){
		case 'N':
			if(!maze[x][y].getUp()){
				if(maze[x-1][y].getValue() < auxValue){
					auxValue = maze[x-1][y].getValue();
					nextDir = 'U';
				}
			}
			if((y < DIM - 1) &&!maze[x][y+1].getLeft()){
				if(maze[x][y+1].getValue() < auxValue){
					auxValue = maze[x][y+1].getValue();
					nextDir = 'R';
				}
			}
			if((x < DIM - 1) && !maze[x+1][y].getUp()){
				if(maze[x+1][y].getValue() < auxValue){
					auxValue = maze[x+1][y].getValue();
					nextDir = 'D';
				}
			}
			if(!maze[x][y].getLeft()){
				if(maze[x][y-1].getValue() < auxValue){
					auxValue = maze[x][y-1].getValue();
					nextDir = 'L';
				}
			}
		break;
		case 'E':
			if((y < DIM - 1) &&!maze[x][y+1].getLeft()){
				if(maze[x][y+1].getValue() < auxValue){
					auxValue = maze[x][y+1].getValue();
					nextDir = 'R';
				}
			}
			if((x < DIM - 1) && !maze[x+1][y].getUp()){
				if(maze[x+1][y].getValue() < auxValue){
					auxValue = maze[x+1][y].getValue();
					nextDir = 'D';
				}
			}
			if(!maze[x][y].getLeft()){
				if(maze[x][y-1].getValue() < auxValue){
					auxValue = maze[x][y-1].getValue();
					nextDir = 'L';
				}
			}
			if(!maze[x][y].getUp()){
				if(maze[x-1][y].getValue() < auxValue){
					auxValue = maze[x-1][y].getValue();
					nextDir = 'U';
				}
			}
		break;
		case 'S':
			if((x < DIM - 1) && !maze[x+1][y].getUp()){
				if(maze[x+1][y].getValue() < auxValue){
					auxValue = maze[x+1][y].getValue();
					nextDir = 'D';
				}
			}
			if(!maze[x][y].getLeft()){
				if(maze[x][y-1].getValue() < auxValue){
					auxValue = maze[x][y-1].getValue();
					nextDir = 'L';
				}
			}
			if(!maze[x][y].getUp()){
				if(maze[x-1][y].getValue() < auxValue){
					auxValue = maze[x-1][y].getValue();
					nextDir = 'U';
				}
			}
			if((y < DIM - 1) &&!maze[x][y+1].getLeft()){
				if(maze[x][y+1].getValue() < auxValue){
					auxValue = maze[x][y+1].getValue();
					nextDir = 'R';
				}
			}
		break;
		case 'W':
			if(!maze[x][y].getLeft()){
				if(maze[x][y-1].getValue() < auxValue){
					auxValue = maze[x][y-1].getValue();
					nextDir = 'L';
				}
			}
			if(!maze[x][y].getUp()){
				if(maze[x-1][y].getValue() < auxValue){
					auxValue = maze[x-1][y].getValue();
					nextDir = 'U';
				}
			}
			if((y < DIM - 1) &&!maze[x][y+1].getLeft()){
				if(maze[x][y+1].getValue() < auxValue){
					auxValue = maze[x][y+1].getValue();
					nextDir = 'R';
				}
			}
			if((x < DIM - 1) && !maze[x+1][y].getUp()){
				if(maze[x+1][y].getValue() < auxValue){
					auxValue = maze[x+1][y].getValue();
					nextDir = 'D';
				}
			}
		break;
	}

	switch(nextDir){
		case 'U':
			if(ori == 'N') choice = 'F';
			else if(ori == 'E') choice = 'L';
			else if(ori == 'S') choice = 'B';
			else if(ori == 'W') choice = 'R';
		break;
		case 'R':
			if(ori == 'N') choice = 'R';
			else if(ori == 'E') choice = 'F';
			else if(ori == 'S') choice = 'L';
			else if(ori == 'W') choice = 'B';
		break;
		case 'D':
			if(ori == 'N') choice = 'B';
			else if(ori == 'E') choice = 'R';
			else if(ori == 'S') choice = 'F';
			else if(ori == 'W') choice = 'L';
		break;
		case 'L':
			if(ori == 'N') choice = 'L';
			else if(ori == 'E') choice = 'B';
			else if(ori == 'S') choice = 'R';
			else if(ori == 'W') choice = 'F';
		break;
	}
}