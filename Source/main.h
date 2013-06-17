#pragma once

#include "includer.h"

extern float PlayerDamageValue(int weap);

//関数
int main();
void var_init();
pos searchNearTeki(pos bulletPos);
class SpjControl{
private:
	int StartFrame;
	void Init(){
		StartFrame=frame;
	}
	EasyTimer Test;
	bool CheckFrame(int TimaerValue){
		if(frame-StartFrame >= TimaerValue) return true;
		return false;
	}

public:

	bool isConverting;
	bool isActivated;

	SpjControl():isConverting(0){
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
				chara::jiki.SpeAmount+=10;
				isConverting=false;
			}
		}

	}
};