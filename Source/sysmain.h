#pragma once

extern unsigned int frame;
extern unsigned int drawFrame;
extern char key[256];
extern int Cred, Cblack, Cblue, Cgreen, Cwhite;
extern int Fsmall,Fnorm;
extern int stage;
extern int ScreenShot;
extern int HighScore;

//デバッグ用
void debugtimer(char* before);

//タイマー
class EasyTimer{

	/*
	EasyTimer	使いやすいタイマーです。
				20個までの独立したタイマーを扱えます。defineして、TimerNumに値を渡してください。

	.Init(タイマーの種類)					タイマーのカウントをリセットします
	.CheckSecond(タイマーの種類, 秒数)		Initから指定時間経過したかチェックします
	.CheckFrame(タイマーの種類, フレーム数) Initから指定フレーム経過したかチェックします
	*/

private:
	int StartSecond[20];
	int StartFrame;

public:


	void Init(int TimerNum){
		StartSecond[TimerNum]=GetNowCount();
		StartFrame=frame;
	}

	bool CheckSecond(int TimerNum, unsigned int TimerValue){
		TimerValue*=1000;
		if(GetNowCount()-StartSecond[TimerNum] >=  TimerValue) return true;
		return false;
	}
	bool CheckFrame(int TimerNum, int TimaerValue){
		if(frame-StartFrame >= TimaerValue) return true;
		return false;
	}

};

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
			WaitTimer(SecondPerFrame-interval);
			nowCount=GetNowCount();
			totalLackTime+=nowCount-PastFrameTime;
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
	FpsStabilizer():SecondPerFrame(16.66666667),PastFrameTime(0),drawSkip(0),maxSkipFrame(4),skiped(false),totalLackTime(0){//コンストラクタ
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
	bool skipedCheck(){//スキップしたかどうか
		return skiped;
	}
};
extern FpsStabilizer FpsStabilizer_Main;
