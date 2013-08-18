#pragma once

#include "includer.h"
namespace chara{
	extern bool isLaserActive;
};
extern float PlayerDamageValue(int weap);
extern EasyTimer Timer_Main;
extern int TimeLine[10][10*60*60*5];
extern int ClearTime;
extern int Result;

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
	int ATK;

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
			if(SpjControl::CheckFrame(100)){
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
				ATK=38;
				break;
			case 1:
				isSLDReady=true;
				SLD=38;
				break;
			}
		}

		if(isSLDReady){
			if(SLD<25){
				isSPEReady=true;
				isSLDReady=false;

			}
			if(frame%30==0) SLD--;
		}
		if(isATKReady){
			if(ATK<25){
				isSPEReady=true;
				isATKReady=false;

			}
			if(frame%30==0) ATK--;			
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

class CommonEnemyTypeA{

public:
	CommonEnemyTypeA():isDead(true),Life(250){}
	bool isDead;
	bool isDamaged;
	float x,y;
	float Life;

	void Move(){
			y+=3;
	}
	void Draw(){
			if(!isDead) DrawGraph(x-71,y,graph::en_c[1],true);
	}

};

	class meteo{
	public:
		meteo():avail(false){}
		bool avail;
		int x,y;
		int num;
	};