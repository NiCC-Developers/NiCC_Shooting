/*
stage1.cpp

複数ステージに対応させるための、各関数を分散させる試みです。
敵の移動・描画・弾発射をここに定義します。他のファイルから関数を呼ぶには、includeするヘッダでexternする必要があります。
また、弾の変数とかを共有してるので衝突防止にnamespace使ってますが、使い方がヘタなので意味を成していません。
*/


#include "includer.h"

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
	if(flag==true) DrawGraph(x-72,y-72,graph::compterve_d,true);

}

float TekiHit_1(){
	const float SPE_MULTI=5;
	float result;
	result=0;
	for(int weap=0; weap<=9; weap++){
		if(weap==MD_WEAP_C){
			for(int i=0;i<MAX_BULLET_NUM;i++){
				if(jikiTrackingBullet[i].avail&&boss[0].x-30<jikiTrackingBullet[i].x && jikiTrackingBullet[i].x<boss[0].x+30 && boss[0].y-10<jikiTrackingBullet[i].y && jikiTrackingBullet[i].y<boss[0].y+10){
					result=PlayerDamageValue(weap);
					jikiTrackingBullet[i].avail=false;
				}
			}
		}
		else{
			for(int i=0;i<=199;i++){
				if(my_bullet[weap][i].avail&&boss[0].x-70<my_bullet[weap][i].x && my_bullet[weap][i].x<boss[0].x+70 && boss[0].y-10<my_bullet[weap][i].y && my_bullet[weap][i].y<boss[0].y+10){
					result=PlayerDamageValue(weap);
					my_bullet[weap][i].avail=false;
				}
			}
		}
	}
	if(isLaserActive){
		if(boss[0].x-70<chara::jiki.x && chara::jiki.x<boss[0].x+70){
			result=PlayerDamageValue(MD_WEAP_C);
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
	
	WriteNobelString("たった今、敵の宇宙戦艦が高濃縮SPE爆弾の起爆準備をしているとの情報が入った。ヤツらに一番近い座標を飛行中のパイロットはお前しかいない。");
	WriteNobelString("タイムリミットは2分。ターゲットを破壊し、地球を死守せよ！");
}

pos searchNearTeki_1(pos bulletPos){
	pos returnVal={boss[0].x,boss[0].y};
	return returnVal;
}