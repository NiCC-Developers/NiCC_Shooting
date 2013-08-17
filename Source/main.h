#pragma once

#include "includer.h"

extern float PlayerDamageValue(int weap);
extern EasyTimer Timer_Main;

extern int TimeLine[10][10*60*60*5];

//関数
int main();
void var_init();
pos searchNearTeki(pos bulletPos);

class SpjControl{
private:

	void Init(){
		StartFrame=frame;
	}
	EasyTimer Test;
	bool CheckFrame(int TimaerValue){
		if(frame-StartFrame >= TimaerValue) return true;
		return false;
	}

public:
	
int StartFrame;
	bool isConverting;
	bool isActivated;
	bool isSPEReady;
	bool isSLDReady;
	bool isATKReady;
	int SLD;

	SpjControl():isConverting(0),isSPEReady(true){
	}

	void Convert(){
		if(key[KEY_INPUT_C]==1) isActivated=true;
		if(chara::jiki.SpjAmount<=0){
			isActivated=false;
			return;
		}
		if(isActivated){
			if(!isConverting){
				SpjControl::Init();
				isConverting=true;
			}
			if(SpjControl::CheckFrame(300)){
				chara::jiki.SpjAmount--;
				chara::jiki.SpeAmount++;
				isConverting=false;
			}
		}
	}

	void Use(){
		if(key[KEY_INPUT_LCONTROL]==1 && chara::jiki.SpeAmount>0 && isSPEReady){
			isSPEReady=false;
			chara::jiki.SpeAmount--;
			switch(chara::jiki.SpeOutput){
			case 0:
				isATKReady=true;
			case 1:
				isSLDReady=true;
				SLD=38;
			}
		}

		if(isSLDReady){
			if(SLD<25){
				isSPEReady=true;
				isSLDReady=false;

			}
			if(frame%30==0) SLD--;
		}

	}

};

class TimeControl{
private:
	int LocalFrame;
	bool isPaused;
	bool isLocked;
public:
	int LocalSecond;
	void Start(){
		if(!isLocked){
			LocalFrame=GetNowCount();
			isPaused=false;
			isLocked=true;
		}
	}
	void Add(){
		if(!isPaused) LocalFrame++;
		if(LocalFrame%6==0) LocalSecond++;
	}
	void Pause(){
		isPaused=true;
	}
	void Resume(){
		isPaused=false;
	}
	void Unlock(){
		isLocked=false;
	}

};

class CommonEnemy{

public:
	CommonEnemy():isDead(true){}
	bool isDead;
	int Ghandle;
	float x,y, Angle;
	float Life, Def, Pwr;

	void Move(){
			y+=3;
	}
	void Draw(){
			if(!isDead) DrawGraph(x,y,graph::en_c[1],true);
	}
	void Attack();
	void Hurt();
};

	class meteo{
	public:
		meteo():avail(false){}
		bool avail;
		int x,y;
		int num;
	};