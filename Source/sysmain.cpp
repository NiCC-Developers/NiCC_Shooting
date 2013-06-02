/*
sysmain.cpp

ここのWinMain関数からスタートします。
main.cppのmainで描画した内容を、ループごとにフリップ・消去します。
mainでは、WaitKeyなどでループから脱出しない場合を除きScreenFlipを行う必要はありません。
*/

#include "includer.h"

//グローバル変数
unsigned int frame=0; //フレーム用変数
double fps=0.0; //FPS
char key[256]; //キーの入力状態格納
int Cred, Cblack, Cblue, Cgreen, Cwhite; //色定義
int Fsmall,Fnorm;
int stage=1; //ステージ番号
unsigned int StartTime;
int ScreenShot;
Save_t ConfigData={100, 100, false};

//関数プロトタイプ宣言
double GetFPS(); 
void SetColor();
void SetFont();


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow ){

	//初期化-----
	LoadConfigData();//設定の読み込み
	if(ConfigData.isFullscreen == true) ChangeWindowMode(false); else ChangeWindowMode(true);
	srand((unsigned)time(NULL)); //乱数のシード値をランダムに指定
	if(DxLib_Init()==-1) return -1; //---------------------------DXLib初期化-----------------------------
	SetDrawScreen(DX_SCREEN_BACK); //裏描画設定
	ScreenShot=MakeGraph(640,480);
	ChangeFontType(DX_FONTTYPE_ANTIALIASING_4X4);
	SetColor(); //色定義
	SetFont(); //フォント定義

	LoadGraphics(); //画像の読み込み
	//初期化終了-----

#define MD_STARTMENU_BEGIN 0
#define MD_STARTMENU_END 1
#define MD_STARTMENU_CONFIG 2
	title:
	switch(StartScreen()){ //スタート画面
	case -1:
		goto end;
		break;
	case 0:
		break; //開始
	case 1:
		GetDrawScreenGraph(0,0,640,480,ScreenShot);
		switch(ConfigScreen()){
		case 0:
			break;
		case 1:
			goto title;
			break;
		}
		break;
	case 2:
		goto end; //終了
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

	FpsStabilizer FpsStabilizer_Main;

	while(ProcessMessage()==0/* && CheckHitKey(KEY_INPUT_ESCAPE)==0*/){
		FpsStabilizer_Main.Do();
		FpsStabilizer_Main.Init();

		fps=GetFPS();
		GetHitKeyStateAll(key);
		ClearDrawScreen();

		//main関数を呼び出し、返り値が0以外なら何か実行
		switch(main()){
		case -1:
			goto end;
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

//FPSを取得する関数
double GetFPS(){
	static double result=0;
	static double tmptime[2];

	if(frame==0){
		tmptime[0]=GetNowCount();
	}
	
	if((frame&31)==0){
		tmptime[1]=GetNowCount();
		result=1000.0f/((tmptime[1]-tmptime[0])/32.0f);
		tmptime[0]=tmptime[1];
	}
	frame++;

	return result;
}

//タイマークラスの関数定義
void c_timer::init(int TimerNum){
	starttime[TimerNum]=GetNowCount();
}
bool c_timer::hasPassed(int TimerNum, unsigned int TimerValue){
	TimerValue*=1000;
	if(GetNowCount()-starttime[TimerNum] >=  TimerValue) return true; else return false;
}

//色定義
void SetColor(){
	Cred = GetColor(200,0,0);
	Cblack = GetColor(0,0,0);
	Cblue = GetColor(0,0,200);
	Cgreen = GetColor(0,200,0);
	Cwhite = GetColor(255,255,255);
}

//フォント定義
void SetFont(){
	Fsmall=CreateFontToHandle(NULL,12,4);
	Fnorm=CreateFontToHandle(NULL,14,4);
}

void FpsStabilizer::Do(){
	if(GetNowCount()-PastFrameTime<SecondPerFrame){
		WaitTimer(SecondPerFrame-(GetNowCount()-PastFrameTime));
	}
}
void FpsStabilizer::Init(){
	PastFrameTime=GetNowCount();
}
FpsStabilizer::FpsStabilizer(){
	//コンストラクタ
	SecondPerFrame=16.66666667;
	PastFrameTime=0;
}