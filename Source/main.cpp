
/*
main.cpp

基本的な処理を書きます。
sysmain.cppのWinMain関数から呼び出されます。
*/


#include "includer.h"

int active_bullets;
bool talkphase=true;
int JikiBulletDamageList[JIKI_BULLET_KIND]={3,1,1,1,1,2};


typedef struct{
	life_t life;
	bool isDamage;
}shield_t;
shield_t shield={4,4,false};

namespace chara{
	bullet_t tb[MAX_BULLET_NUM]; //敵弾
	bullet_t tmb[MAX_BULLET_NUM]; //弾移動
	bullet_t jb[MAX_BULLET_NUM]; //自機弾
	TrackingBullet jikiTrackingBullet[MAX_BULLET_NUM];
	bullet_t my_bullet[JIKI_BULLET_KIND][MAX_BULLET_NUM];
	bullet_t jmb[MAX_BULLET_NUM]; //使ってない
	jiki_t jiki={320,400,4,5,5,0,false,false};
	teki_t boss[20]={
		{320,30,100,100,false},
	};
}

namespace sys{
	//square_t framesize={0,0,420,480};
	square_t framesize={0,0,640,480};
}

using namespace chara;
using namespace sys;

//関数プロトタイプ宣言
void Move();
void TBulletMove();
void JBulletMove();
bool isJikiHit();
int TekiDamage();
void Draw();
void JikiDamage(int DamageValue);

c_timer Timer;

//メインループ---------------------------------
int main(){
	DrawGraph(0,0,graph::back[0],true);

	if(talkphase==true){
		ShowNobel();
		talkphase=false;
	}

	Move();
	JBulletMove();
	if(isJikiHit()==true){
		//jiki.life.now-=1;
		JikiDamage(1);
		jiki.damage=true;
		shield.isDamage=true;
		Timer.init(MD_TIMER_SHIELD);
	}else{
		jiki.damage=false;
		shield.isDamage=false;
	}
	TBulletMove();
	if(jiki.life.now <=0) return 1;
	if(boss[0].life.now<=0) return 2;
	int damage=TekiDamage();
	boss[0].damage=(bool)damage;
	boss[0].life.now-=damage;

	active_bullets=0;
	for(int i=0;i<100;i++){
		if(tb[i].avail==true) active_bullets++;
	}
	Draw();

	static bool past_push=true;
	if(key[KEY_INPUT_ESCAPE]==1 && past_push==false){
		GetDrawScreenGraph(0,0,640,480,ScreenShot,false);
		if(PauseGame()==-1) return -1;
	}
	if(CheckHitKeyAll()==0) past_push=false; else past_push=true;
}
//---------------------------------------------


void Draw(){
	//ボス
	switch(stage){
	case 1:
		TekiDraw_1(boss[0].x,boss[0].y,boss[0].damage);
		break;
	}
	//自機
	if(key[KEY_INPUT_LEFT]==1){
		DrawGraph(jiki.x-48,jiki.y-48,graph::hdmaru[1],true);
	} else if(key[KEY_INPUT_RIGHT]==1){
		DrawGraph(jiki.x-48,jiki.y-48,graph::hdmaru[2],true);
	} else DrawGraph(jiki.x-48,jiki.y-48,graph::hdmaru[0],true);
	if(key[KEY_INPUT_Z]==1){
		DrawGraph(jiki.x-48,jiki.y-48,graph::hdmaru[3],true);
	}

	//シールド

	static bool isChargeTime=false;
	if(isChargeTime==false){
		Timer.init(MD_TIMER_SHIELD);
		isChargeTime=true;
	}
	
	if(Timer.hasPassed(MD_TIMER_SHIELD,5)==true){
		if(shield.life.now!=shield.life.max) shield.life.now+=1;
		isChargeTime=false;
	}

	if(shield.life.now > 0){
		if(shield.isDamage==false){
			DrawCircle(jiki.x,jiki.y,70,GetColor(255-255*(float)shield.life.now/(float)shield.life.max,0,0+255*(float)shield.life.now/(float)shield.life.max),false);
		} else {
			DrawCircle(jiki.x,jiki.y,68,Cwhite,false);
		}
	}

	//自機体力
	DrawBox(jiki.x-40,jiki.y+55,jiki.x+40,jiki.y+60,GetColor(255,255,255),false);
	DrawBox(jiki.x-40,jiki.y+55,jiki.x-40+80*(float)jiki.life.now/(float)jiki.life.max,jiki.y+60,GetColor(255,255,255),true);

	if(jiki.damage==true) DrawBox(jiki.x-10,jiki.y-10,jiki.x+10,jiki.y+10,Cred,true);
	if(jiki.ahantei==true){
		DrawCircle(jiki.x,jiki.y,2,Cred,true);
	}

	//武器切り替え
	DrawFormatString(jiki.x-40,jiki.y+40,Cwhite,"WEAP:%d",jiki.weap);

	if(key[KEY_INPUT_X]==1){
		static bool past_pushed=1;
	} else {

	}

	if(key[KEY_INPUT_LCONTROL]==1){
		static bool past_pushed;
		static double pangle=0;
		static int rotate_flag; //0=NO 1=LEFT 2=RIGHT

		if(key[KEY_INPUT_LEFT]==1){
			if(past_pushed==false){
				past_pushed=true;
				jiki.weap+=1;
				jiki.weap=jiki.weap%4;
				rotate_flag=1;
			}
		} else if(key[KEY_INPUT_RIGHT]==1){
			if(past_pushed==false){
				past_pushed=true;
				jiki.weap-=1;
				if(jiki.weap<0) jiki.weap=3;
				jiki.weap=jiki.weap%4;
				rotate_flag=2;
			}
		} else {
			past_pushed=false;
		}

		if(rotate_flag==0){
			DrawWeaponCircle(jiki.x,jiki.y,jiki.weap*90,jiki.weap);
		}

		if(rotate_flag==1){
			if(pangle<=90){
				DrawWeaponCircle(jiki.x,jiki.y,(jiki.weap-1)*90+pangle,jiki.weap);
				pangle+=10;
			}
			if(pangle==90){
				pangle=0;
				rotate_flag=0;
			}
		}

		if(rotate_flag==2){
			if(pangle>=-90){
				DrawWeaponCircle(jiki.x,jiki.y,(jiki.weap+1)*90+pangle,jiki.weap);
				pangle-=10;
			}
			if(pangle==-90){
				pangle=0;
				rotate_flag=0;
			}
		}
	}

	for(int i=0;i<200;i++){
		if(tb[i].avail==true){
			DrawCircle(tb[i].x,tb[i].y,3,Cgreen,true);
		}
	}

	for(int weap=0; weap<=9; weap++){
		for(int i=0;i<200;i++){
			if(my_bullet[weap][i].avail==true){
				//DrawBox(my_bullet[weap][i].x-3,my_bullet[weap][i].y-3,my_bullet[weap][i].x+3,my_bullet[weap][i].y+3,GetColor(255,255,255),true);
				if(weap==0) DrawGraph(my_bullet[weap][i].x-8,my_bullet[weap][i].y-8,graph::bullet[0],true);
				if(2<=weap && weap<=4) DrawGraph(my_bullet[weap][i].x-4,my_bullet[weap][i].y-4,graph::bullet[1],true);
			}
		}
	}
	//WEAP_C
	for(int i=0;i<MAX_BULLET_NUM;i++){
		if(jikiTrackingBullet[i].avail==false)continue;
		DrawGraph(jikiTrackingBullet[i].x-8,jikiTrackingBullet[i].y-8,graph::bullet[2],true);
	}



	//GUI
	DrawBox(framesize.right,0,640,480,GetColor(255,255,255),true);
	DrawBox(0,0,640*boss[0].life.now/boss[0].life.max,10,Cred,true);
	DrawFormatString(framesize.right+20,20,Cred,"弾幕STG.Prototype");
	DrawFormatString(framesize.right+20,100,Cblack,"お前のやる気：%d",jiki.life.now);
	DrawFormatString(framesize.right+20,120,Cblack,"ボム：使えません");
	DrawFormatString(framesize.right+20,140,Cblack,"Score：すごい");
	DrawFormatString(framesize.right+20,160,Cblack,"HiScore：知らないです");

	DrawFormatString(framesize.right+20,180,Cblack,"座標：(%d,%d)",jiki.x,jiki.y);
	DrawFormatString(framesize.right+20,200,Cblack,"アクティブな弾:%d/100",active_bullets);

	DrawFormatString(framesize.right+20,240,Cblue,"移動：方向ｷｰ(ｼﾌﾄでｽﾛｰ)");
	DrawFormatString(framesize.right+20,260,Cblue,"ショット：Z");
	DrawFormatString(framesize.right+20,280,Cblue,"やめる：ESC");

	DrawFormatString(framesize.right+20,300,Cblack,"敵のやる気：%d",boss[0].life.now);
	DrawFormatString(framesize.right+20,340,Cred,"frame：%d",frame);
	

}

void Move(){
	//自機移動

	if(key[KEY_INPUT_LCONTROL]==0){
		if(key[KEY_INPUT_LSHIFT]==1){
			jiki.speed=3;
			jiki.ahantei=true;
		}else{
			jiki.speed=6;
			jiki.ahantei=false;
		}
		if(key[KEY_INPUT_LEFT]==1){
			jiki.x-=jiki.speed;
		}
		if(key[KEY_INPUT_RIGHT]==1){
			jiki.x+=jiki.speed;
		}
		if(key[KEY_INPUT_UP]==1){
			jiki.y-=jiki.speed;
		}
		if(key[KEY_INPUT_DOWN]==1){
			jiki.y+=jiki.speed;
		}
	}
	
	//敵移動
	switch(stage){
	case 1:
		TekiMove_1(&boss[0].x,&boss[0].y);
		break;

	}

	//自機画面外処理
	if(jiki.x>framesize.right) jiki.x=framesize.right;
	if(jiki.x<framesize.left) jiki.x=framesize.left;
	if(jiki.y>framesize.bottom) jiki.y=framesize.bottom;
	if(jiki.y<framesize.top) jiki.y=framesize.top;

}

void TBulletMove(){

	//敵弾発射
	switch(stage){
	case 1:
		TekiBullet_1();
		break;

	}

}

void JBulletMove(){
	//自機弾発射関数
	static int max=0;
	static int bullet_num[10];
	const int WEAP_A=0;
	const int WEAP_B=1;
	const int WEAP_C=2;
	const int WEAP_D=3;

	//弾召喚
	switch (jiki.weap){
	case WEAP_A:
		if(key[KEY_INPUT_Z]==1 && frame%4==1){
			if(bullet_num[MD_WEAP_A]>=199) bullet_num[MD_WEAP_A]=0;
			bullet_num[MD_WEAP_A]++;
			if(my_bullet[MD_WEAP_A][bullet_num[MD_WEAP_A]].avail==false){
				my_bullet[MD_WEAP_A][bullet_num[MD_WEAP_A]].avail=true;
				my_bullet[MD_WEAP_A][bullet_num[MD_WEAP_A]].x=jiki.x;
				my_bullet[MD_WEAP_A][bullet_num[MD_WEAP_A]].y=jiki.y;
			}
		}
		break;
	case WEAP_B:
		if(key[KEY_INPUT_Z]==1 && frame%2==1){
			if(bullet_num[MD_WEAP_B_WAY1]>=199) bullet_num[MD_WEAP_B_WAY1]=0;
			bullet_num[MD_WEAP_B_WAY1]++;
			if(my_bullet[MD_WEAP_B_WAY1][bullet_num[MD_WEAP_B_WAY1]].avail==false){
				my_bullet[MD_WEAP_B_WAY1][bullet_num[MD_WEAP_B_WAY1]].avail=true;
				my_bullet[MD_WEAP_B_WAY1][bullet_num[MD_WEAP_B_WAY1]].x=jiki.x;
				my_bullet[MD_WEAP_B_WAY1][bullet_num[MD_WEAP_B_WAY1]].y=jiki.y;
			}

			if(bullet_num[MD_WEAP_B_WAY2]>=199) bullet_num[MD_WEAP_B_WAY2]=0;
			bullet_num[MD_WEAP_B_WAY2]++;
			if(my_bullet[MD_WEAP_B_WAY2][bullet_num[MD_WEAP_B_WAY2]].avail==false){
				my_bullet[MD_WEAP_B_WAY2][bullet_num[MD_WEAP_B_WAY2]].avail=true;
				my_bullet[MD_WEAP_B_WAY2][bullet_num[MD_WEAP_B_WAY2]].x=jiki.x;
				my_bullet[MD_WEAP_B_WAY2][bullet_num[MD_WEAP_B_WAY2]].y=jiki.y;
			}

			if(bullet_num[MD_WEAP_B_WAY3]>=199) bullet_num[MD_WEAP_B_WAY3]=0;
			bullet_num[MD_WEAP_B_WAY3]++;
			if(my_bullet[MD_WEAP_B_WAY3][bullet_num[MD_WEAP_B_WAY3]].avail==false){
				my_bullet[MD_WEAP_B_WAY3][bullet_num[MD_WEAP_B_WAY3]].avail=true;
				my_bullet[MD_WEAP_B_WAY3][bullet_num[MD_WEAP_B_WAY3]].x=jiki.x;
				my_bullet[MD_WEAP_B_WAY3][bullet_num[MD_WEAP_B_WAY3]].y=jiki.y;
			}
		}
		break;
	case WEAP_C:
		if(key[KEY_INPUT_Z]==1 && frame%16==1){
			bullet_num[MD_WEAP_C]++;
			bullet_num[MD_WEAP_C]&=MAX_BULLET_NUM-1;
			if(jikiTrackingBullet[bullet_num[MD_WEAP_C]].avail)break;
			jikiTrackingBullet[bullet_num[MD_WEAP_C]].avail=true;
			jikiTrackingBullet[bullet_num[MD_WEAP_C]].x=jiki.x;
			jikiTrackingBullet[bullet_num[MD_WEAP_C]].y=jiki.y;
			jikiTrackingBullet[bullet_num[MD_WEAP_C]].angle=-PI/2;
		}
		break;
	default:
		break;
	}
	DrawFormatString(0,20,Cwhite,"%d",bullet_num[MD_WEAP_A]);
	DrawFormatString(0,40,Cwhite,"%d",my_bullet[MD_WEAP_A][bullet_num[MD_WEAP_A]].avail);

	//弾移動
	//WEAP_A
	for(int i=0; i<200; i++){
		if(my_bullet[MD_WEAP_A][i].avail==true) my_bullet[MD_WEAP_A][i].y-=15;
		if(my_bullet[MD_WEAP_A][i].x > framesize.right+6 || my_bullet[MD_WEAP_A][i].x < framesize.left-6 || my_bullet[MD_WEAP_A][i].y < framesize.top-6 || my_bullet[MD_WEAP_A][i].y > framesize.bottom+6){
			my_bullet[MD_WEAP_A][i].avail=false;
		}
	}

	//WEAP_B
	for(int i=0; i<200; i++){
		if(my_bullet[MD_WEAP_B_WAY1][i].avail==true) my_bullet[MD_WEAP_B_WAY1][i].y-=15;
		if(my_bullet[MD_WEAP_B_WAY1][i].x > framesize.right+100 || my_bullet[MD_WEAP_B_WAY1][i].x < framesize.left-100 || my_bullet[MD_WEAP_B_WAY1][i].y < framesize.top-100 || my_bullet[MD_WEAP_B_WAY1][i].y > framesize.bottom+100){
			my_bullet[MD_WEAP_B_WAY1][i].avail=false;
		}
	}
	for(int i=0; i<200; i++){
		if(my_bullet[MD_WEAP_B_WAY2][i].avail==true){
			my_bullet[MD_WEAP_B_WAY2][i].x+=15*cos(PI/180*70);
			my_bullet[MD_WEAP_B_WAY2][i].y-=15*sin(PI/180*70);
		}
		if(my_bullet[MD_WEAP_B_WAY2][i].x > framesize.right+100 || my_bullet[MD_WEAP_B_WAY2][i].x < framesize.left-100 || my_bullet[MD_WEAP_B_WAY2][i].y < framesize.top-100 || my_bullet[MD_WEAP_B_WAY2][i].y > framesize.bottom+100){
			my_bullet[MD_WEAP_B_WAY2][i].avail=false;
		}
	}

	for(int i=0; i<200; i++){
		if(my_bullet[MD_WEAP_B_WAY3][i].avail==true){
			my_bullet[MD_WEAP_B_WAY3][i].x+=15*cos(PI/180*110);
			my_bullet[MD_WEAP_B_WAY3][i].y-=15*sin(PI/180*110);
		}
		if(my_bullet[MD_WEAP_B_WAY3][i].x > framesize.right+100 || my_bullet[MD_WEAP_B_WAY3][i].x < framesize.left-100 || my_bullet[MD_WEAP_B_WAY3][i].y < framesize.top-100 || my_bullet[MD_WEAP_B_WAY3][i].y > framesize.bottom+100){
			my_bullet[MD_WEAP_B_WAY3][i].avail=false;
		}
	}
	//WEAP_C
	for(int i=0;i<MAX_BULLET_NUM;i++){
		jikiTrackingBullet[i].move();
	}

}

bool isJikiHit(){
	//自機当たり判定
	static bool result;
	result=false;
	for(int i=0;i<=199;i++){
		if(sqrt(pow(tb[i].x-jiki.x,2)+pow(tb[i].y-jiki.y,2)) < 6){
			tb[i].avail=false;
			result=true;
		}
	}
	return result;
}

int TekiDamage(){
	switch(stage){
	case 1:
		return TekiHit_1();
		break;

	default:
		return false;
		break;
	}
}

void var_init(){
	//変数初期化

	jiki.x=320,jiki.y=400;
	boss[0].x=320,boss[0].y=30;
	jiki.speed=4;
	jiki.ahantei=false; /*当たり判定表示フラグ*/
	jiki.life.now=jiki.life.max;
	shield.life.now=shield.life.max;
	boss[0].life.now=boss[0].life.max;

	for(int i=0;i<MAX_BULLET_NUM;i++){
		jb[i].x=-1;
		jb[i].y=-1;
		jb[i].avail=false;
		tb[i].x=-1;
		tb[i].y=-1;
		tb[i].avail=false;

		jikiTrackingBullet[i].avail=false;
	}
	for(int weap=0;weap<JIKI_BULLET_KIND;weap++){
		for(int i=0;i<MAX_BULLET_NUM;i++){
			my_bullet[weap][i].avail=false;
		}
	}
}

void JikiDamage(int damage){

	if(shield.life.now-damage <= 0){
		jiki.life.now-=damage-shield.life.now;
		shield.life.now=0;
	} else {
		shield.life.now-=damage;
	}
}

pos searchNearTeki(pos bulletPos){
	switch(stage){
	case 1:
		return searchNearTeki_1(bulletPos);
	}
}
void TrackingBullet::move(){
	if(avail==false)return;
	pos bulletPos={x,y};
	pos TekiPos=searchNearTeki(bulletPos);
	float relativeAngle=-angle+atan2(TekiPos.y-y,TekiPos.x-x);
	//回転量が小さくなるように調整
	if(relativeAngle>PI){
		relativeAngle-=2*PI;
	}
	else if(relativeAngle<-PI){
		relativeAngle+=2*PI;
	}
	//回転制限
	relativeAngle=min(maxLotate,max(-maxLotate,relativeAngle));
	angle+=relativeAngle;
	//-PI<angle<PIになるように
	if(angle>PI){
		angle-=2*PI;
	}
	else if(relativeAngle<-PI){
		angle+=2*PI;
	}
	y+=TrackingBulletSpeed*sin(angle);
	x+=TrackingBulletSpeed*cos(angle);
	return;
}


/*

Q.コードの書き方が汚いです

A.
　':,　　　　 ',　　　_____,,.. -‐ ''"´￣￣｀"'' ｰ　､.,　　　　　　　　 　／
　　':,　　　　',　　 ＞'　´　　　　　　　　　　　　　｀ヽ.　　　　　　 /　　　　知
　　　':,　　　　 ／　　　　　　　　　　　　　　　　　 　 ヽ.　　　　 ,'　　    　 っ
　　　　':,　　 ,:' ／　　 ／　 　,'´　　　　　　　 ヽ.　　 　 ':,/Ti　 i.　　そ　て
.　＼　　　　,' /　　　/　 ,'　　!　　　　 　;　 　',　 ヽ__　／::::| |　|　　 ん　る
　　　＼　 / ,'　　　,'!　 /!　　!　 　;　　/!　　　i　 「:::|'´::::::::| |　.!.　 な  わ
　　　　 ∠__,!　　 / !メ､」_,,./| 　 /!　/　!　　 ﾊ!　|__」＜:::::」」　|.　　 こ  よ
｀"''　 ､..,,_　 !　 /　,ｧ7´, `iヽ|　/　|ヽ､」ﾆイ､　|　 !　|^ヽ､」」 　|.　　 と
　　　　　　　i,／ﾚｲ　i┘　i.　ﾚ' 　 'ｱ´!_」 ハヽ|　　　|　　　| ∠　　　 ! !
─--　 　 　/　　 !　 ゝ- '　　　　 　　! 　　 !　!　　　|　　　|　　｀ヽ.
　　　　　　/　 　7/l/l/　　　､　　 　　`'ｰ‐ '_ノ!　　　|　 i　 |　 　　｀ ' ｰ---
,. -──-'､　　,人　　　　｀i`ｧｰ-- ､　　/l/l/l |　　　 !.　|　 |
　　　　　　 ヽ.ｿ　 `: ､. 　　ﾚ'　　　　', 　 u　,/|　　　 |　 !　 |
　と　　今　　i　　/ｰﾅ= ､ '､　　　　ﾉ　　,.イ,ｶ　　　 !　 |　 |
　こ　　修  　.|ﾍ./|／レへ｀＞-r　 =ﾆi´､.,_　|　 i　 ﾊ　 !　,'
　ろ　　正　　 !　　　　 _,.イ´ヽ.7　　 ／　 /:::|　/ﾚ'　 ﾚ'ﾚ'
　な　　し　　 | 　　／7:::::!　　○Ｏ'´　　/::::::ﾚ'ヽ.
　の　  て　　.|　 /　 /:::::::レ'/ムヽ.　　/::::::::/ 　 ヽ.
　! !　　る　　 ! ./　 ,':::::::::::!/　ハ:::::｀´:::::::::::;'　　　　',

*/