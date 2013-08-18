/*
ui.cpp

スタート画面などのUIを呼び出す関数です。
*/

#include "includer.h"

namespace chara{
	extern jiki_t jiki;
}

typedef struct{
	char name[20];
	int color;
	int x;
	int y;
	int value;
} menuelm;


int StartScreen(){
	char key[256];
	int sel=0; //現在選択している項目
	int decision=-2; //決定した項目
	int title_h=LoadGraph("res\\gui\\title.png"); //タイトル
	int back=LoadGraph("res\\background\\title_FailOver.png");
	bool past_push=false; //ボタン押しっぱなし回避
	int ELEM_NUM=2; //メニュー要素数
	int STR_LINE=100; //メニューX座標

	//メニュー内容
	static menuelm menu[]={ 
		{"Start",Cwhite,STR_LINE,500,},
		//{"Config",Cwhite,STR_LINE,550,},
		{"Exit",Cwhite,STR_LINE,600,},
	};
	
	FpsStabilizer FpsStabilizer_Start;
	while(ProcessMessage()==0 && decision==-2){
		FpsStabilizer_Start.Do();
		FpsStabilizer_Start.skip();
		ClearDrawScreen();
		GetHitKeyStateAll(key);
		if(past_push==false){ //メニュー移動
			if(key[KEY_INPUT_UP]==1) sel-=1;
			if(key[KEY_INPUT_DOWN]==1) sel+=1;
			if(key[KEY_INPUT_Z]==1){
				past_push=true;
				decision=sel;
			}
			//ループ処理
			if(sel < 0) sel=ELEM_NUM-1;
			if(sel > ELEM_NUM-1) sel=0;
		}
			if(CheckHitKeyAll()==0) past_push=false; else past_push=true; //ボタン押しっぱなし回避
			
			//メニュー描画
			for(int i=0; i<ELEM_NUM; i++){
				if(i==sel) {
					DrawFormatString(menu[i].x-20,menu[i].y,Cred,menu[i].name);
					menu[i].x-=5;
					if(STR_LINE-menu[i].x > 20) menu[i].x=STR_LINE-20;
				}
				else {
					DrawFormatString(menu[i].x,menu[i].y,menu[i].color,menu[i].name);
					menu[i].x+=5;
					if(STR_LINE <= menu[i].x) menu[i].x=STR_LINE;
				}
			}
			DrawGraph(0,0,back,true);
			DrawStringToHandle(10,460,"Press arrow key to move, Z key to select ",Cwhite,Fnorm);
			DrawFormatString(875,0,Cwhite,"HighScore:%dsec",HighScore);
			ScreenFlip();
	}
	DeleteGraph(title_h);
	if(ProcessMessage()==-1) decision=-1;
	return decision; //選択した番号を返す
}

int ConfigScreen(){


	char key[256]; //キー押下状態格納
	int sel=0; //現在選択している項目
	int decision=-2; //決定した項目
	int title_h=LoadGraph("res\\gui\\title.png"); //タイトル画像
	bool past_push=true; //ボタン押しっぱなし回避
	int bgmvol=100; //BGMのボリューム *未実装
	int sevol=100; //SEのボリューム *未実装
	bool isfullscreen=false; //フルスクリーンモード

	const int ELEM_NUM=3; //メニュー要素数
	const int STR_LINE=200; //メニューX座標
	const int CFG_BGM=0; //BGMは0番目
	const int CFG_SE=1; //SEは1番目
	const int CFG_FULLSCREEN=2; //フルスクリーンは2番目

	Save_t tmpConfigData=ConfigData;

	//メニュー内容
	menuelm menu[3]={
		{"BGM Vol",Cwhite,STR_LINE,200, bgmvol}, {"SE Vol",Cwhite,STR_LINE,250, sevol}, {"Full Screen",Cwhite,STR_LINE,300, isfullscreen}
	};

	FpsStabilizer FpsStabilizer_Config;

	while(ProcessMessage()==0 && decision==-2){
		FpsStabilizer_Config.Do();
		FpsStabilizer_Config.Init();
		ClearDrawScreen();
		DrawGraph(0,0,ScreenShot,false);
		DrawGraph(100,100,graph::config_back,true);
		GetHitKeyStateAll(key);

		//----メニュー内移動----
		//いっぱい動かしたいものはここに
		if(sel == CFG_BGM) {
			if(key[KEY_INPUT_LEFT]==1 && tmpConfigData.bgmvol>0) tmpConfigData.bgmvol--;
			if(key[KEY_INPUT_RIGHT]==1 && tmpConfigData.bgmvol<100) tmpConfigData.bgmvol++;
		}
		if(sel == CFG_SE){
			if(key[KEY_INPUT_LEFT]==1) tmpConfigData.sevol--;
			if(key[KEY_INPUT_RIGHT]==1) tmpConfigData.sevol++;
		}
		//ちまちま動かしたいものはここに
		if(past_push==false){ 
			if(key[KEY_INPUT_UP]==1) sel-=1;
			if(key[KEY_INPUT_DOWN]==1) sel+=1;
			if(sel == CFG_FULLSCREEN){
				if(key[KEY_INPUT_LEFT]==1 || key[KEY_INPUT_RIGHT]==1) tmpConfigData.isFullscreen=!tmpConfigData.isFullscreen;

			}
			if(key[KEY_INPUT_RETURN]==1){
				ConfigData=tmpConfigData;
				SaveConfigData();
				return 1;
			}
			if(key[KEY_INPUT_ESCAPE]==1){
				tmpConfigData=ConfigData;
				return 1;
			}

			//ループ処理
			if(sel < 0) sel=ELEM_NUM-1;
			if(sel > ELEM_NUM-1) sel=0;
		}
			if(CheckHitKeyAll()==0) past_push=false; else past_push=true; //ボタン押しっぱなし回避
		//----メニュー内移動ここまで----

			menu[CFG_BGM].value=tmpConfigData.bgmvol;
			menu[CFG_SE].value=tmpConfigData.sevol;
			menu[CFG_FULLSCREEN].value=tmpConfigData.isFullscreen;

			//メニュー描画
			for(int i=0; i<ELEM_NUM; i++){
				if(i==sel) {
					DrawFormatString(menu[i].x-20,menu[i].y,Cred,menu[i].name);
					menu[i].x-=5;
					if(STR_LINE-menu[i].x > 20) menu[i].x=STR_LINE-20;
				}
				else {
					DrawFormatString(menu[i].x,menu[i].y,menu[i].color,menu[i].name);
					menu[i].x+=5;
					if(STR_LINE <= menu[i].x) menu[i].x=STR_LINE;
				}
			}

			for(int i=0; i<ELEM_NUM; i++){
				DrawFormatString(menu[i].x+200, menu[i].y, menu[i].color,"%d", menu[i].value);
			}
			
			DrawFormatString(110,110,Cwhite,"Enterで保存、ESCでキャンセル");
			if(ConfigData.isFullscreen!=tmpConfigData.isFullscreen){
				DrawFormatString(20,440,Cwhite,"フルスクリーンモードを変更するには再起動が必要です");
			}

			ScreenFlip();
	}

	DeleteGraph(title_h);
	if(ProcessMessage()==-1) decision=-1;
	return decision; //選択した番号を返す
}

int PauseGame(){

	char key[256];
	int sel=0; //現在選択している項目
	int decision=-2; //決定した項目
	bool past_push=true; //ボタン押しっぱなし回避
	int ELEM_NUM=3; //メニュー要素数
	int STR_LINE=400; //メニューX座標
	const int PAUSE_RESUME=0;
	const int PAUSE_CONFIG=1;
	const int PAUSE_EXIT=2;
	int back_x=640;

	//メニュー内容
	static menuelm menu[]={ 
		{"再開",Cwhite,STR_LINE,200,},
		{"オプション",Cwhite,STR_LINE,250,},
		{"ゲームの終了",Cwhite,STR_LINE,300,},
	};
	
	FpsStabilizer FpsStabilizer_Pause;
	while(ProcessMessage()==0 && decision==-2){
		FpsStabilizer_Pause.Do();
		FpsStabilizer_Pause.skip();

		ClearDrawScreen();
		DrawGraph(0,0,ScreenShot,false);

		DrawGraph(back_x,0,graph::pause_back,true);
		if(back_x>290) back_x-=40;

		GetHitKeyStateAll(key);
		if(past_push==false){ //メニュー移動
			if(key[KEY_INPUT_UP]==1) sel-=1;
			if(key[KEY_INPUT_DOWN]==1) sel+=1;
			if(key[KEY_INPUT_ESCAPE]==1) decision=PAUSE_RESUME;
			if(key[KEY_INPUT_Z]==1){
				past_push=true;
				decision=sel;
			}
			//ループ処理
			if(sel < 0) sel=ELEM_NUM-1;
			if(sel > ELEM_NUM-1) sel=0;
		}
			if(CheckHitKeyAll()==0) past_push=false; else past_push=true; //ボタン押しっぱなし回避
			
			//メニュー描画
			for(int i=0; i<ELEM_NUM; i++){
				if(i==sel) {
					DrawFormatString(menu[i].x-20,menu[i].y,Cred,menu[i].name);
					menu[i].x-=5;
					if(STR_LINE-menu[i].x > 20) menu[i].x=STR_LINE-20;
				}
				else {
					DrawFormatString(menu[i].x,menu[i].y,menu[i].color,menu[i].name);
					menu[i].x+=5;
					if(STR_LINE <= menu[i].x) menu[i].x=STR_LINE;
				}
			}
			
			ScreenFlip();
	}

	switch (decision){
	case PAUSE_RESUME:
		return 0;
		break;
	case PAUSE_CONFIG:
		ConfigScreen();
		PauseGame();
		break;
	case PAUSE_EXIT:
		return -1;
		break;
	default:
		break;
	}
}

void ShowNobel(){
	switch(stage){
	case 1:
		ShowNobel_1();
		break;

	default:
		break;
	}
}

void DrawWeaponCircle(int posx,int posy,double rotate_angle, int sel_weap){
	static bool isStop;
	if (rotate_angle==sel_weap*90) isStop=true; else isStop=false;
	rotate_angle=(rotate_angle+225)*PI/180;
	posx+=80;
	int hankei=50;

	DrawCircle(posx,posy,10,Cwhite,false);


	DrawCircle(posx,posy,hankei,Cwhite,false);
	if(sel_weap==0 && isStop==true){ DrawCircle(posx+hankei*cos(rotate_angle),posy-hankei*sin(rotate_angle),40,Cwhite,false); } else {DrawCircle(posx+hankei*cos(rotate_angle),posy-hankei*sin(rotate_angle),20,Cwhite,false);}
	if(sel_weap==1 && isStop==true){ DrawCircle(posx+hankei*cos(rotate_angle-0.5*PI),posy-hankei*sin(rotate_angle-0.5*PI),40,Cred,false); } else { DrawCircle(posx+hankei*cos(rotate_angle-0.5*PI),posy-hankei*sin(rotate_angle-0.5*PI),20,Cred,false); }
	if(sel_weap==2 && isStop==true){ DrawCircle(posx+hankei*cos(rotate_angle-1*PI),posy-hankei*sin(rotate_angle-1*PI),40,Cblue,false); } else { DrawCircle(posx+hankei*cos(rotate_angle-1*PI),posy-hankei*sin(rotate_angle-1*PI),20,Cblue,false); }
	if(sel_weap==3 && isStop==true){ DrawCircle(posx+hankei*cos(rotate_angle-1.5*PI),posy-hankei*sin(rotate_angle-1.5*PI),40,Cgreen,false); } else { DrawCircle(posx+hankei*cos(rotate_angle-1.5*PI),posy-hankei*sin(rotate_angle-1.5*PI),20,Cgreen,false); }
}