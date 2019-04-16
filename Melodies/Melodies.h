#ifndef Melodies_h
#define Melodies_h

#include "Arduino.h"

const float FIT_MARIO = 1.4;
const float MARIO_FREC = 1;
const float FIT_PIRATE = 1.4;
const float PIRATE_FREC = 3;
const int BEEP_FREC = 1500;
const int BEEP_DUR = 100;

const char pirates[] = {
		"boucoudovdovdoueoufovfovfougoueoveovdoucoucoudow"
		"boucoudovdovdoueoufovfovfougoueoveovdoucoudox"
		"boucoudovdovdoufougovgovgouhouioviovhougouhou"
		"dowdoueoufovfovgovhou"
		"dowdoufoueoveovfoudoueox"
		"houjoukovkovkouloumovmovmounoulovlovkoujoujoukow"
		"houjoukovkovkouloumovmovmounoulovlovkoujoukox"
		"houjoukovkovkoumounovnovnourousovsovrounouroukow"
		"kouloumovmovnovroukowkoumoulovlovkoutoukovkovlovmovmoumounovrqx"
		"moukouhqysqxmoukouiqydoubovdowzoxnounou"
		"."};
		
const char marioTheme[] = {"iouiouaouiouaougouiouaoukouaouaouaoucouaouaouaougouaouaoucouaouaoubouaouaou"
		"douaoufouaoueoudouaoucpvipvkpvlouaoujoukouaouiouaougouhoufouaouaougouaouaoucouaouaouboua"
		"ouaoudouaoufouaoueoudouaoucpvipvkpvlouaoujoukouaouiouaougouhoufouaouaou"
		"."};
	
typedef struct {
  int freq;
  int dur;
  int del;
}tNote;

class Melodies{
public:
	Melodies(int buzzer);
	void playMario();
	void playPirates();
	void playBeep();
private:
	int buzzer;
	unsigned long currentTime;
	unsigned long previousTime;
	int counter;
	bool songEnd;
	tNote songNote;
	char songParse[3];
	tNote parseMario(char parseNote[3]);
	tNote parsePirates(char parseNote[3]);
};

#endif

