
#include "Melodies.h"
#include "Arduino.h"

Melodies::Melodies(int pin){
		buzzer = pin;
		counter = 0;
		songEnd = false;
		currentTime = 0;
		previousTime = 0;
}

void Melodies::playMario(){
	currentTime = millis();
		if(currentTime - previousTime >= songNote.del * FIT_MARIO){
		songParse[0] = marioTheme[counter];
		songParse[1] = marioTheme[counter+1];
		songParse[2] = marioTheme[counter+2];
		songNote = parseMario(songParse);
		if(marioTheme[counter+3] == '.') songEnd = true;
		tone(buzzer, songNote.freq * MARIO_FREC, songNote.dur * FIT_MARIO);
		previousTime = currentTime;
		if(!songEnd) counter += 3;
		else{
		 counter = 0;
		 songEnd = false;
		 }
	}
}

void Melodies::playPirates(){
	currentTime = millis();
	if(currentTime - previousTime >= songNote.del * FIT_PIRATE){
		songParse[0] = pirates[counter];
		songParse[1] = pirates[counter+1];
		songParse[2] = pirates[counter+2];
		songNote = parsePirates(songParse);
		if(pirates[counter+3] == '.') songEnd = true;
		tone(buzzer, songNote.freq * PIRATE_FREC, songNote.dur * FIT_PIRATE);
		previousTime = currentTime;
		if(!songEnd) counter += 3;
		else{
		 counter = 0;
		 songEnd = false;
		 }
	}
}

void Melodies::playBeep(){
	tone(buzzer, BEEP_FREC, BEEP_DUR);
}

tNote Melodies::parseMario(char parseNote[3]){
    tNote note;
    switch(parseNote[0]){
      case 'a': note.freq = 0; break;
      case 'b': note.freq = 1319; break;
      case 'c': note.freq = 1568; break;
      case 'd': note.freq = 1760; break;
      case 'e': note.freq = 1865; break;
      case 'f': note.freq = 1976; break;
      case 'g': note.freq = 2093; break;
      case 'h': note.freq = 2349; break;
      case 'i': note.freq = 2637; break;
      case 'j': note.freq = 2794; break;
      case 'k': note.freq = 3136; break;
      case 'l': note.freq = 3520; break;
      case 'm': note.freq = 0; break;
      case 'n': note.freq = 0; break;    
    }    
    switch(parseNote[1]){
      case 'o': note.dur = 83; break;
      case 'p': note.dur = 111; break;
      case 'q': note.dur = 0; break;
      case 'r': note.dur = 0; break;
      case 's': note.dur = 0; break;
      case 't': note.dur = 0; break;
    }
     switch(parseNote[2]){
      case 'u': note.del = 107; break;
      case 'v': note.del = 144.44; break;
      case 'w': note.del = 0; break;
      case 'x': note.del = 0; break;
      case 'y': note.del = 0; break;
      case 'z': note.del = 0; break;
    }
    return note;
  }
  
  
tNote Melodies::parsePirates(char parseNote[3]){
    tNote note;
    switch(parseNote[0]){
      case 'a': note.freq = 293.66; break;
      case 'b': note.freq = 440; break;
      case 'c': note.freq = 523.25; break;
      case 'd': note.freq = 587.33; break;
      case 'e': note.freq = 659.25; break;
      case 'f': note.freq = 698.45; break;
      case 'g': note.freq = 783.99; break;
      case 'h': note.freq = 880;    break;
      case 'i': note.freq = 932.33; break;
      case 'j': note.freq = 1046.50; break;
      case 'k': note.freq = 1174.66; break;
      case 'l': note.freq = 1318.51; break;
      case 'm': note.freq = 1396.91; break;
      case 'n': note.freq = 1567.98; break;    
      case 'r': note.freq = 1760; break;
      case 's': note.freq = 1864.66; break;
      case 't': note.freq = 1108.73; break;
      case 'z': note.freq = 554.36; break;
    }    
    switch(parseNote[1]){
      case 'o': note.dur = 100; break;
      case 'p': note.dur = 200; break;
      case 'q': note.dur = 300; break;
    }
     switch(parseNote[2]){
      case 'u': note.del = 100; break;
      case 'v': note.del = 200; break;
      case 'w': note.del = 300; break;
      case 'x': note.del = 400; break;
      case 'y': note.del = 600; break;
    }
    return note;
 }