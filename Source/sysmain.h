#pragma once

extern unsigned int frame;
extern char key[256];
extern int Cred, Cblack, Cblue, Cgreen, Cwhite;
extern int Fsmall,Fnorm;
extern int stage;
extern int ScreenShot;
extern void InitTimer();
extern bool isTimePassed(unsigned int TimerValue_Sec);

class FpsStabilizer{
public:
	FpsStabilizer();
	void Do();
	void Init();
private:
	float SecondPerFrame;
	float PastFrameTime;
};