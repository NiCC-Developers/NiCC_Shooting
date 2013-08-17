/*
sysmain.cpp
ゲームの大部分で使用する関数等を定義します。
ここのWinMain関数からプログラムがスタートするので、初期化したい内容等を記述してください。
ゲームの1フレーム内での内容をmain.cpp内のmain()に記述し、ここでループさせます。

ChangeWindowMode()など、DxLib_Init()を呼ぶ前にしか変更できない物もあるので初期化の順番に注意してください。
*/

#include "includer.h"

//--------------------グローバル変数 --------------------
unsigned int frame=0,drawFrame=0; //現在のフレーム数をカウント
double fps=0.0,cpu_fps=0.0; //FPS
char key[256]; //キーの入力状態格納
int Cred, Cblack, Cblue, Cgreen, Cwhite; //GetColor()の代わりに使用出来る色セット
int Fsmall,Fnorm; //大きさなどを自分で定義したフォントのハンドル。DrawStringToHandle()などで呼び出せます。リファレンス参照のこと
int stage=1; //現在のステージ番号
unsigned int StartTime; 
int ScreenShot; //スクショ保存用ハンドル
Save_t ConfigData={100, 100, false}; //コンフィグデータ初期値
FpsStabilizer FpsStabilizer_Main;

//-------------------関数プロトタイプ宣言 -------------------
double GetFPS(); //FPSを取得
double GetFPS_CPU(); //FPS(CPU処理レート)を取得
void SetColor(); //色セットを定義
void SetFont(); //カスタムフォントデータを作成


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow ){

	//-------------------初期化ここから-------------------
	LoadConfigData();//設定の読み込み
	LoadMapData();
	if(ConfigData.isFullscreen == true) ChangeWindowMode(false); else ChangeWindowMode(true);
	srand((unsigned)time(NULL)); //乱数のシード値をランダムに指定
	SetGraphMode(1024,768,32);
	//------------------- これより下はDxLib初期化済み -------------------
	if(DxLib_Init()==-1) return -1;
	SetDrawScreen(DX_SCREEN_BACK); //裏描画設定
	ScreenShot=MakeGraph(640,480);
	ChangeFontType(DX_FONTTYPE_ANTIALIASING_4X4);
	SetColor(); //色セットの定義
	SetFont(); //カスタムフォントデータを作成
	LoadGraphics(); //画像の読み込み
	//-------------------初期化終了-------------------

#define MD_STARTMENU_BEGIN 0
#define MD_STARTMENU_END 1
#define MD_STARTMENU_CONFIG 2
title:

	//スタート画面
	switch(StartScreen()){
	case -1:
		goto end;
		break;
	case 0:
		break; //開始
	case 1:
		//ポップアップウィンドウを表現するためにスクショを取得
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
	PlayMusic("res\\bgm\\battle.wav",DX_PLAYTYPE_LOOP);
	while(1){
		//FpsStabilizer_Main.Init();
		cpu_fps=GetFPS_CPU();
		if(!FpsStabilizer_Main.skip()){
			if(ProcessMessage()!=0){
				break;
			}
			GetHitKeyStateAll(key);
			FpsStabilizer_Main.Do();
			fps=GetFPS();
			ClearDrawScreen();
		}
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
		if(!FpsStabilizer_Main.skipedCheck()){
			DrawFormatString(0,450,GetColor(255,255,255),"%.1f FPS(CPU:%.1f)",fps,cpu_fps);
			ScreenFlip();
		}
	}

end:

	//終了処理
	DxLib_End();
	return 0;
}

//FPSを取得する関数(CPU処理レート)
double GetFPS_CPU(){
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
double GetFPS(){
	static double result=0;
	static double tmptime[2];

	if(drawFrame==0){
		tmptime[0]=GetNowCount();
	}
	
	if((drawFrame&31)==0){
		tmptime[1]=GetNowCount();
		result=1000.0f/((tmptime[1]-tmptime[0])/32.0f);
		tmptime[0]=tmptime[1];
	}
	drawFrame++;

	return result;
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


void debugtimer(char* before){
	static int past;
	int now=GetNowCount();
	char cash[50];
	itoa(now-past,cash,10);
	OutputDebugString(before);
	OutputDebugString(cash);
	OutputDebugString("\n");
	past=now;
	return;
}