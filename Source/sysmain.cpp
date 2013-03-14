/*
sysmain.cpp

ここのWinMain関数からスタートします。
main.cppのmainで描画した内容を、ループごとにフリップ・消去します。
mainでは、WaitKeyなどでループから脱出しない場合を除きScreenFlipを行う必要はありません。
*/

#include "common.h"

//グローバル変数
unsigned short int frame=0; //フレーム用変数
double fps=0.0; //FPS
char key[256]; //キーの入力状態格納
int Cred, Cblack, Cblue, Cgreen, Cwhite;
int Fsmall,Fnorm;
int stage=1; //ステージ番号
unsigned int StartTime;

//関数プロトタイプ宣言
double GetFPS(); 
void SetColor();
void SetFont();


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow ){

	//初期化-----
	srand((unsigned)time(NULL)); //乱数のシード値をランダムに指定
	ChangeWindowMode(true);
	if(DxLib_Init()==-1) return -1;
	SetDrawScreen(DX_SCREEN_BACK);
	SetColor();
	SetFont();
	LoadGraphics();
	//初期化終了-----

	title:
	switch(StartScreen()){
	case -1:
		goto end;
		break;
	case 0:
		break;
	case 1:
		goto end;
		break;
	default:
		DrawFormatString(200,220,Cblue,"ERROR: フラグが定義されてないよう…");
		ScreenFlip();
		WaitKey();
		goto end;
		break;
	}

	//メインループ
	start:

	while(ProcessMessage()==0 && CheckHitKey(KEY_INPUT_ESCAPE)==0){

		fps=GetFPS();
		GetHitKeyStateAll(key);
		ClearDrawScreen();

		//main関数を呼び出し、返り値が0以外なら何か実行
		switch(main()){
		case 1:
			DrawGraph(300,10,graph::chara[2],true);
			DrawFormatString(320,200,GetColor(255,255,255),"死んだ");
			ScreenFlip();
			WaitKey();
			var_init();
			goto start;
			break;
		case 2:
			DrawGraph(300,10,graph::chara[1],true);
			DrawFormatString(320,220,GetColor(255,255,255),"すごい");
			ScreenFlip();
			WaitKey();
			var_init();
			goto start;
			break;
		}

		DrawFormatString(0,450,GetColor(255,255,255),"%.1f FPS",fps);

		ScreenFlip();
	}

end:

	//終了処理
	DxLib_End();
	return 0;
}

double GetFPS(){
	//FPSを取得する関数
	static double result=0;
	static double tmptime[2];

	if(frame==0){
		tmptime[0]=GetNowCount();
	}
	
	if(frame>=29){
		tmptime[1]=GetNowCount();
		result=1000.0f/((tmptime[1]-tmptime[0])/30.0f);
		frame=0;
	}else frame++;

	return result;
}

void InitTimer(){
	StartTime=GetNowCount();
}

bool  isTimePassed(unsigned int TimerValue){
	TimerValue*=1000;
	if(GetNowCount()-StartTime >=  TimerValue) return true; else return false;
}

void SetColor(){
	Cred = GetColor(200,0,0);
	Cblack = GetColor(0,0,0);
	Cblue = GetColor(0,0,200);
	Cgreen = GetColor(0,200,0);
	Cwhite = GetColor(255,255,255);
}

void SetFont(){
	Fsmall=CreateFontToHandle(NULL,12,4);
	Fnorm=CreateFontToHandle(NULL,14,4);
}