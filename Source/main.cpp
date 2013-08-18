
/*
main.cpp

基本的な処理を書きます。
sysmain.cppのWinMain関数から呼び出されます。
*/

#include "includer.h"

int active_bullets; //有効な弾
bool talkphase=true; //テキストダイアログが出るか
float JikiBulletDamageList[JIKI_BULLET_KIND]={15,4,4,4,4,0.01}; //各弾のダメージ値
float TekiMaxLotate=PI/180*1;
float TekiTrackingBulletSpeed=4;
int buf[200];
int ClearTime;
int Result;
meteo Meteo[100];

typedef struct{
	life_t life;
	bool isDamage;
}shield_t;
shield_t shield={5,5,false};

namespace chara{
	bullet_t tb[MAX_BULLET_NUM]; //敵弾
	bullet_t tmb[MAX_BULLET_NUM]; //弾移動
	bullet_t tbzA[MAX_BULLET_NUM];
	bullet_t tmbzA[MAX_BULLET_NUM];
	bullet_t jb[MAX_BULLET_NUM]; //自機弾
	TrackingBullet jikiTrackingBullet[MAX_BULLET_NUM]; //ホーミング弾
	bullet_t my_bullet[JIKI_BULLET_KIND][MAX_BULLET_NUM];
	bool isLaserActive;
	bullet_t jmb[MAX_BULLET_NUM]; //使ってない
	jiki_t jiki={320,400,4,15,15,0,0,2,0,false,false};
	teki_t boss[20]={
		{320,30,10000,10000,false},
	};
}

namespace sys{
	//square_t framesize={0,0,420,480};
	square_t framesize={0,0,1024,768};
}

const int ENM_MAX_LINE=10;
const int ENM_MAX_TIME=10*60*60*5;
CommonEnemyTypeA ETA[ENM_MAX_LINE][ENM_MAX_TIME]; //敵さん！

//CommonEnemyTypeA ET2[ENM_MAX_LINE][ENM_MAX_TIME]; //敵さん（B）！
int TimeLine[ENM_MAX_LINE][ENM_MAX_TIME]={0}; //時間-位置を表す、敵を出さない場合は0


using namespace chara;
using namespace sys;

//関数プロトタイプ宣言
void Move(); //自機と敵の移動
void TBulletMove(); //敵弾の移動
void JBulletMove(); //自機弾の移動
bool isJikiHit(); //自機の当たり判定
float TekiDamage(); //敵へのダメージ
void Draw(); //描画
void JikiDamage(int DamageValue); //自機へのダメージ
void DropEnemy();
void MoveEnemy();
void DrawEnemy();
void HitEnemy();
void DrawBackground();

EasyTimer Timer_Main; //タイマー
SpjControl SpjControl_Main; //SPJとSPEの操作
TimeControl TC_Main;

//メインループ---------------------------------
int main(){
	TC_Main.Start();
	TC_Main.Add();

	DropEnemy();
	MoveEnemy();
	HitEnemy();

	if(!FpsStabilizer_Main.skipedCheck()){

	}
	//ノベルモードに入るかどうか
	if(talkphase==true){
		ShowNobel();
		talkphase=false;
	}

	{
		static bool isLocked;
		if(key[KEY_INPUT_SPACE]){
			if(!isLocked){
 				jiki.SpeOutput+=1;
				jiki.SpeOutput%=2;
			}
			isLocked=true;
		} else {
			isLocked=false;
		}
	}

	if(key[KEY_INPUT_S]) jiki.SpeOutput=1;
	if(key[KEY_INPUT_A]) jiki.SpeOutput=0;
	if(key[KEY_INPUT_3]) jiki.SpeOutput=3;

	Move();
	JBulletMove();
	SpjControl_Main.Convert();
	SpjControl_Main.Use();

	//自機弾ヒット時の処理
	if(isJikiHit()==true){
		//jiki.life.now-=1;
		JikiDamage(1);
		jiki.damage=true;
		shield.isDamage=true;
		//Timer.init(MD_TIMER_SHIELD);
		Timer_Main.Init(TIMER_SHIELD);
		
	}else{
		jiki.damage=false;
		shield.isDamage=false;
	}
	TBulletMove();

	//ゲーム終了
	if(jiki.life.now <=0) {
		Result=0;
		return 1;
	}
	if(boss[0].life.now<=0){
		ClearTime=TC_Main.LocalSecond/10;
		return 2;
	}
	if(TC_Main.LocalSecond>1200){
		Result=1;
		return 1;
	}
	float damage=TekiDamage();
	boss[0].damage=(bool)damage;
	boss[0].life.now-=damage;
	active_bullets=0;
	for(int i=0;i<100;i++){
		if(tb[i].avail==true) active_bullets++;
	}
	if(!FpsStabilizer_Main.skipedCheck()){
		//スキップしない
		DrawBackground();
		Draw();
		DrawEnemy();
	}//
		//ポーズ開始処理
		static bool past_push=true;
		if(key[KEY_INPUT_ESCAPE]==1 && past_push==false){
			/*GetDrawScreenGraph(0,0,640,480,ScreenShot,false);
			if(PauseGame()==-1) return -1;*/
		}
		if(CheckHitKeyAll()==0) past_push=false; else past_push=true;
	//}
}
//---------------------------------------------

//描画関数
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
		Timer_Main.Init(TIMER_SHIELD);
		//Timer.init(MD_TIMER_SHIELD);
		isChargeTime=true;
	}
	
	if(Timer_Main.CheckSecond(TIMER_SHIELD,2)==true){
		if(shield.life.now!=shield.life.max) shield.life.now+=1;
		isChargeTime=false;
	}

	if(shield.life.now > 0 && SpjControl_Main.isSLDReady){
		if(shield.isDamage==false){
			DrawCircle(jiki.x,jiki.y,70,GetColor(255-255*(float)shield.life.now/(float)shield.life.max,0,0+255*(float)shield.life.now/(float)shield.life.max),false);
		} else {
			DrawCircle(jiki.x,jiki.y,68,Cwhite,false);
		}
	}

	//自機体力
	/*DrawBox(jiki.x-40,jiki.y+55,jiki.x+40,jiki.y+60,GetColor(255,255,255),false);
	DrawBox(jiki.x-40,jiki.y+55,jiki.x-40+80*(float)jiki.life.now/(float)jiki.life.max,jiki.y+60,GetColor(255,255,255),true);*/

	if(jiki.damage==true) DrawBox(jiki.x-10,jiki.y-10,jiki.x+10,jiki.y+10,Cred,true);
	if(jiki.ahantei==true){
		DrawCircle(jiki.x,jiki.y,2,Cred,true);
	}

	//武器切り替え
	//DrawFormatString(jiki.x-40,jiki.y+40,Cwhite,"WEAP:%d",jiki.weap);
	
	static bool past_pushed=1;
	if(key[KEY_INPUT_X]==1){
		if(!past_pushed){
			jiki.weap++;
			jiki.weap%=3;
			past_pushed=true;
		}
	} else {
		past_pushed=false;
	}

	//if(key[KEY_INPUT_LCONTROL]==1){
	//	static bool past_pushed;
	//	static double pangle=0;
	//	static int rotate_flag; //0=NO 1=LEFT 2=RIGHT

	//	if(key[KEY_INPUT_LEFT]==1){
	//		if(past_pushed==false){
	//			past_pushed=true;
	//			jiki.weap+=1;
	//			jiki.weap=jiki.weap%4;
	//			rotate_flag=1;
	//		}
	//	} else if(key[KEY_INPUT_RIGHT]==1){
	//		if(past_pushed==false){
	//			past_pushed=true;
	//			jiki.weap-=1;
	//			if(jiki.weap<0) jiki.weap=3;
	//			jiki.weap=jiki.weap%4;
	//			rotate_flag=2;
	//		}
	//	} else {
	//		past_pushed=false;
	//	}

	//	if(rotate_flag==0){
	//		DrawWeaponCircle(jiki.x,jiki.y,jiki.weap*90,jiki.weap);
	//	}

	//	if(rotate_flag==1){
	//		if(pangle<=90){
	//			DrawWeaponCircle(jiki.x,jiki.y,(jiki.weap-1)*90+pangle,jiki.weap);
	//			pangle+=10;
	//		}
	//		if(pangle==90){
	//			pangle=0;
	//			rotate_flag=0;
	//		}
	//	}

	//	if(rotate_flag==2){
	//		if(pangle>=-90){
	//			DrawWeaponCircle(jiki.x,jiki.y,(jiki.weap+1)*90+pangle,jiki.weap);
	//			pangle-=10;
	//		}
	//		if(pangle==-90){
	//			pangle=0;
	//			rotate_flag=0;
	//		}
	//	}
	//}

	//弾描画
	for(int i=0;i<200;i++){
		if(tb[i].avail==true){
			DrawGraph(tb[i].x-6,tb[i].y-6,graph::enm_bullet1,true);
			//DrawCircle(tb[i].x,tb[i].y,3,Cgreen,true);
		}
	}

	for(int weap=0; weap<=9; weap++){
		for(int i=0;i<200;i++){
			if(my_bullet[weap][i].avail==true){
				if(weap==0) DrawGraph(my_bullet[weap][i].x-8,my_bullet[weap][i].y-8,graph::bullet[0],true);
				if(2<=weap && weap<=4) DrawGraph(my_bullet[weap][i].x-4,my_bullet[weap][i].y-4,graph::bullet[1],true);
			}
		}
	}
	if(isLaserActive){
		DrawLine(jiki.x,jiki.y,jiki.x,0,Cgreen,true);
	}
	//WEAP_C
	/*for(int i=0;i<MAX_BULLET_NUM;i++){
		if(jikiTrackingBullet[i].avail==false)continue;
		DrawGraph(jikiTrackingBullet[i].x-8,jikiTrackingBullet[i].y-8,graph::bullet[2],true);
	}*/
	//GUI
	DrawGraph(0,0,graph::ui_main,true); //メイン
	DrawGraph(0,0,graph::ui_hidariue,true); //左上
	DrawGraph(0,0,graph::ui_weap[jiki.weap+1],true); //武器
	DrawGraph(0,0,graph::ui_spj[jiki.SpjAmount],true); //SPJ残り
	{ //変換アニメーションとメーター
		static int i;
		if(SpjControl_Main.isConverting) i=frame%100;
		if(0<=i && i<25) DrawGraph(0,0,graph::ui_spj_convert[3],true);
		if(25<=i && i<50) DrawGraph(0,0,graph::ui_spj_convert[2],true);
		if(50<=i && i<75) DrawGraph(0,0,graph::ui_spj_convert[1],true);
		if(75<=i && i<100) DrawGraph(0,0,graph::ui_spj_convert[0],true);
		if(SpjControl_Main.isConverting) DrawCircleGauge(262,667,(float)(frame-SpjControl_Main.StartFrame)/100*100,graph::ui_spj_time);
	}

	DrawCircleGauge(896,614,(float)jiki.life.now/jiki.life.max*100,graph::ui_weap_meter); //体力メーター
	DrawGraph(0,0,graph::ui_spe_out[jiki.SpeOutput],true); //spe出力先
	DrawGraph(0,0,graph::ui_spe[jiki.SpeAmount],true); //spe残り
	switch(chara::jiki.SpeOutput){
	case 0:
		DrawCircleGauge(910,620,SpjControl_Main.ATK,graph::ui_spe_meter);
		break;
	case 1:
		DrawCircleGauge(910,620,SpjControl_Main.SLD,graph::ui_spe_meter); //SPEメーター、38%で最大 25%で最小
		break;
	}
	{
		static bool isLocked;
		if(!SpjControl_Main.isSPEReady){

			static int SPEPowerX;
			if(!isLocked){
				SPEPowerX=320;
				shield.life.now=shield.life.max;
				isLocked=true;
			}

			if(SPEPowerX<807){
					SPEPowerX+=40;
			}
			DrawBox(320,762,SPEPowerX,763,Cgreen,true);
		} else {
			isLocked=false;
		}

	}
	DrawFormatString(framesize.left+17,37,Cwhite,"地球滅亡まで: %d秒",120-(int)TC_Main.LocalSecond/10);

	DrawBox(0,0,1024*boss[0].life.now/boss[0].life.max,5,Cred,true);
	
}

void Move(){
	//自機移動

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

	//ボス
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
		isLaserActive=false;
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
		isLaserActive=false;
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
		
		if(key[KEY_INPUT_Z]==1){
			isLaserActive=true;
		} else {
			isLaserActive=false;
		}

		//ホーミング
		//if(key[KEY_INPUT_Z]==1 && frame%16==1){
		//	bullet_num[MD_WEAP_C]++;
		//	bullet_num[MD_WEAP_C]&=MAX_BULLET_NUM-1;
		//	if(jikiTrackingBullet[bullet_num[MD_WEAP_C]].avail)break;
		//	jikiTrackingBullet[bullet_num[MD_WEAP_C]].avail=true;
		//	jikiTrackingBullet[bullet_num[MD_WEAP_C]].x=jiki.x;
		//	jikiTrackingBullet[bullet_num[MD_WEAP_C]].y=jiki.y;
		//	jikiTrackingBullet[bullet_num[MD_WEAP_C]].angle=-PI/2;
		//}
		break;
	default:
		break;
	}

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
	for(int i=0; i<200; i++){
		if(my_bullet[MD_WEAP_C][i].avail==true) my_bullet[MD_WEAP_C][i].y-=15;
		if(my_bullet[MD_WEAP_C][i].x > framesize.right+6 || my_bullet[MD_WEAP_C][i].x < framesize.left-6 || my_bullet[MD_WEAP_C][i].y < framesize.top-6 || my_bullet[MD_WEAP_C][i].y > framesize.bottom+6){
			my_bullet[MD_WEAP_C][i].avail=false;
		}
	}

	//ホーミング
	//for(int i=0;i<MAX_BULLET_NUM;i++){
	//	jikiTrackingBullet[i].JikiMove();
	//}

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

float TekiDamage(){
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
	jiki.SpjAmount=6;
	jiki.SpeAmount=0;
	jiki.SpeOutput=0;
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

	for(int n=0; n<198; n+=2){
		ETA[buf[n]][buf[n+1]].isDead=true;
	}
}

void JikiDamage(int damage){

	if(SpjControl_Main.isSLDReady){
		if(shield.life.now-damage <= 0){
			jiki.life.now-=damage-shield.life.now;
			shield.life.now=0;
		} else {
			shield.life.now-=damage;
		}
	} else {
		jiki.life.now-=damage;
	}
}
pos searchNearTeki(pos bulletPos){
	switch(stage){
	case 1:
		return searchNearTeki_1(bulletPos);
	}
}
void TrackingBullet::JikiMove(){
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
void TrackingBullet::TekiMove(){
	if(avail==false)return;
	float relativeAngle=-angle+atan2(chara::jiki.y-y,chara::jiki.x-x);
	//回転量が小さくなるように調整
	if(relativeAngle>PI){
		relativeAngle-=2*PI;
	}
	else if(relativeAngle<-PI){
		relativeAngle+=2*PI;
	}
	//回転制限
	relativeAngle=min(TekiMaxLotate,max(-TekiMaxLotate,relativeAngle));
	angle+=relativeAngle;
	//-PI<angle<PIになるように
	if(angle>PI){
		angle-=2*PI;
	}
	else if(relativeAngle<-PI){
		angle+=2*PI;
	}
	y+=TekiTrackingBulletSpeed*sin(angle);
	x+=TekiTrackingBulletSpeed*cos(angle);
	return;
}

float PlayerDamageValue(int weap){
	const int SPE_MULTI=3;
	if(SpjControl_Main.isATKReady) return JikiBulletDamageList[weap]*SPE_MULTI; else return JikiBulletDamageList[weap];
}

void DropEnemy(){
	static int Counter;
	for(int n=0; n<ENM_MAX_LINE; n++){
		
		switch(TimeLine[n][TC_Main.LocalSecond]){
		case 1:
			if(ETA[n][TC_Main.LocalSecond].isDead){
				ETA[n][TC_Main.LocalSecond].x=1024*n/(ENM_MAX_LINE-1);
				ETA[n][TC_Main.LocalSecond].y=-250;
				ETA[n][TC_Main.LocalSecond].isDead=false;
				buf[Counter%200]=n;
				buf[Counter%200+1]=TC_Main.LocalSecond;
				Counter+=2;
			}
			break;
		default:
			break;
		}
		
	}
}

void MoveEnemy(){
	for(int i=0; i<ENM_MAX_LINE; i++){
		for(int j=0; j<ENM_MAX_TIME; j++){
			switch(TimeLine[i][j]){
			case 1:
				if(!ETA[i][j].isDead){
					ETA[i][j].Move();
				}

			}
		}
	}

}

void HitEnemy(){
	for(int n=0; n<=198; n+=2){
		for(int k=0; k<MAX_BULLET_NUM; k++){
			if(ETA[buf[n]][buf[n+1]].isDead==false){

				if(my_bullet[MD_WEAP_A][k].avail==true && ETA[buf[n]][buf[n+1]].x-40<my_bullet[MD_WEAP_A][k].x && my_bullet[MD_WEAP_A][k].x<ETA[buf[n]][buf[n+1]].x+40 && ETA[buf[n]][buf[n+1]].y-50<my_bullet[MD_WEAP_A][k].y && my_bullet[MD_WEAP_A][k].y<ETA[buf[n]][buf[n+1]].y+50){
					my_bullet[MD_WEAP_A][k].avail=false;
					ETA[buf[n]][buf[n+1]].Life-=PlayerDamageValue(MD_WEAP_A);
					ETA[buf[n]][buf[n+1]].isDamaged=true;
					if(ETA[buf[n]][buf[n+1]].Life<=0){
						ETA[buf[n]][buf[n+1]].isDead=true;
						if(GetRand(5)==1 && chara::jiki.SpjAmount<=6) chara::jiki.SpjAmount++;
					}
				}
				for(int weap=2; weap<=4; weap++){
					if(my_bullet[weap][k].avail==true && ETA[buf[n]][buf[n+1]].x-40<my_bullet[weap][k].x && my_bullet[weap][k].x<ETA[buf[n]][buf[n+1]].x+40 && ETA[buf[n]][buf[n+1]].y-50<my_bullet[weap][k].y && my_bullet[weap][k].y<ETA[buf[n]][buf[n+1]].y+50){
						my_bullet[weap][k].avail=false;
						ETA[buf[n]][buf[n+1]].Life-=PlayerDamageValue(weap);
						ETA[buf[n]][buf[n+1]].isDamaged=true;
						if(ETA[buf[n]][buf[n+1]].Life<=0){
							ETA[buf[n]][buf[n+1]].isDead=true;
							if(GetRand(5)==1 && chara::jiki.SpjAmount<=6) chara::jiki.SpjAmount++;
						}
					}
				}

				if(isLaserActive && ETA[buf[n]][buf[n+1]].x-40 < jiki.x && jiki.x < ETA[buf[n]][buf[n+1]].x+40){
					ETA[buf[n]][buf[n+1]].Life-=PlayerDamageValue(MD_WEAP_C);
					ETA[buf[n]][buf[n+1]].isDamaged=true;
					if(ETA[buf[n]][buf[n+1]].Life<=0){
						ETA[buf[n]][buf[n+1]].isDead=true;
						if(GetRand(5)==1 && chara::jiki.SpjAmount<=6) chara::jiki.SpjAmount++;
					}
				}
			}
		}
	}

}

void DrawEnemy(){
	for(int n=0; n<=198; n+=2){
		if(!ETA[buf[n]][buf[n+1]].isDead){
			if(ETA[buf[n]][buf[n+1]].isDamaged){
				DrawGraph(ETA[buf[n]][buf[n+1]].x-71,ETA[buf[n]][buf[n+1]].y-50,graph::en_c_d[1],true);
				ETA[buf[n]][buf[n+1]].isDamaged=false;
			} else {
				DrawGraph(ETA[buf[n]][buf[n+1]].x-71,ETA[buf[n]][buf[n+1]].y-50,graph::en_c[1],true);
			}
		}
	}
}

void DrawBackground(){

	//----背景----
	static int LOne=-768;
	static int LTwo=-768;
	DrawGraph(0,LOne,graph::back[0],true);
	DrawGraph(0,LTwo,graph::back[1],true);
	if(LOne>0) LOne=-768; else if(frame%3==0) LOne++;
	if(LTwo>0) LTwo=-768;
	else if(frame%2==0) LTwo++;

	//----隕石部分----
	static int Counter;
	if(frame%200==GetRand(199)){
		Meteo[Counter].avail=true;
		Meteo[Counter].x=(float)1024*GetRand(10)/10;
		Meteo[Counter].y=-200;
		Meteo[Counter].num=GetRand(2);
		Counter++;
		Counter%=100;
	}

	for(int i=0; i<100; i++){
		if(Meteo[i].y>800){
			Meteo[i].avail=false;
		}
		
		if(Meteo[i].avail){
			Meteo[i].y+=2;
			DrawGraph(Meteo[i].x,Meteo[i].y,graph::bg_meteo[Meteo[i].num],true);
		}
	}
	//----隕石ここまで----
}