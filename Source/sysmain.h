#pragma once

extern unsigned int frame;
extern unsigned int drawFrame;
extern char key[256];
extern int Cred, Cblack, Cblue, Cgreen, Cwhite;
extern int Fsmall,Fnorm;
extern int stage;
extern int ScreenShot;
extern void InitTimer();
extern bool isTimePassed(unsigned int TimerValue_Sec);

//デバッグ用
void debugtimer(char* before);

//FPS安定化クラス
class FpsStabilizer{
	float SecondPerFrame;//目標値
	int maxSkipFrame;//最大連続スキップ数
	int PastFrameTime;
	int drawSkip;//連続スキップ数
	float totalLackTime;//足りていない時間合計
	bool skiped;
public:
	void Do(){
		auto nowCount=GetNowCount();
		auto interval=nowCount-PastFrameTime;
		if(interval<SecondPerFrame){//時間あまり
			WaitTimer(SecondPerFrame-(nowCount-PastFrameTime));
			totalLackTime+=SecondPerFrame;
		}
		else {//時間不足(orちょうど=0)
			totalLackTime+=interval;
			totalLackTime=min(totalLackTime,SecondPerFrame*maxSkipFrame);
		}
		PastFrameTime=nowCount;
	}
	void Init(){
		PastFrameTime=GetNowCount();
	}
	FpsStabilizer():SecondPerFrame(16.66666667),PastFrameTime(0),drawSkip(0),maxSkipFrame(4),skiped(false){//コンストラクタ
	}
	bool skip(){
		totalLackTime-=SecondPerFrame;
		if(totalLackTime>SecondPerFrame&&drawSkip<=maxSkipFrame){
			//スキップする
			drawSkip++;
			skiped=true;
			return true;
		}
		else{
			drawSkip=0;
			skiped=false;
			return false;
		}
	}
	bool skipedCheck(){
		return skiped;
	}
};
extern FpsStabilizer FpsStabilizer_Main;
