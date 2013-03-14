/*
stage1.cpp

複数ステージに対応させるための、各関数を分散させる試みです。
敵の移動・描画・弾発射をここに定義します。他のファイルから関数を呼ぶには、includeするヘッダでexternする必要があります。
また、弾の変数とかを共有してるので衝突防止にnamespace使ってますが、使い方がヘタなので意味を成していません。
*/


#include "common.h"
#include "stage1_p.h"

using namespace chara;
using namespace sys;

void TekiMove_1(int *x,int *y){
	boss[0].y=70;
	if(*x>framesize.right+30){
		*x=framesize.left-30;
	}else if(*x<framesize.left-30){
		*x=framesize.right+30;
	}else{
		*x+=3;
	}

}

void TekiDraw_1(int x,int y,bool flag){
	DrawBox(x-30,y-15,x+30,y+15,Cred,true);
	DrawGraph(x-72,y-72,graph::compterve,true);
	if(flag==true) DrawBox(x-30,y-15,x+30,y+15,GetColor(255,255,255),true);

}

bool TekiHit_1(){
	static bool result;
	result=false;
	for(int i=0;i<=199;i++){
		if(boss[0].x-30<jb[i].x && jb[i].x<boss[0].x+30 && boss[0].y-10<jb[i].y && jb[i].y<boss[0].y+10){
			result=true;
		}
	}
	return result;

}

void TekiBullet_1(){
	static int dist;
	static float angle;

	for(int i=0;i<100;i++){

		if(tb[i].avail==false){
			dist=2+GetRand(4);
			angle=1+GetRand(358);
			angle=angle/180;
			tmb[i].x= dist*cos(angle);
			tmb[i].y= dist*sin(angle);
			tb[i].avail=true;
			tb[i].x=boss[0].x;
			tb[i].y=boss[0].y;
		}
		
		if(tb[i].x > framesize.right+6 || tb[i].x < framesize.left-6 || tb[i].y < framesize.top-6 || tb[i].y > framesize.bottom+6){
			tb[i].avail=false;
		}else{

		}

		if(tb[i].avail==true){
			tb[i].x=tb[i].x+tmb[i].x;
			tb[i].y+=tmb[i].y;
		}

	}

}

void ShowNobel_1(){
	SetDrawChara(1,1);
	
	WriteNobelString("俺の名は阪上 蓮紺！");
	WriteNobelString("因みにレンコンはあんまり好きじゃないぜ！");
}