/*
common.h

共通ヘッダファイル
*/

#pragma once

//#DEFINE

const unsigned int JIKI_BULLET_KIND=10;
const unsigned int MAX_BULLET_NUM=256;
extern float JikiBulletDamageList[JIKI_BULLET_KIND];
#define PI 3.141592
#define MD_WEAP_A 0
#define MD_WEAP_B 1
#define MD_WEAP_B_WAY1 2
#define MD_WEAP_B_WAY2 3
#define MD_WEAP_B_WAY3 4
#define MD_WEAP_C 5
#define MD_WEAP_D 6
#define TIMER_WORLD 0
#define TIMER_SHIELD 1
#define TIMER_SPJ_CONVERT 2

//弾クラス
typedef struct{
	float x;
	float y;
	bool avail;
}bullet_t;
//追尾弾,MD_WEAP_C
const float maxLotate=PI/180*3;
const float TrackingBulletSpeed=5;
extern float TekiMaxLotate;
extern float TekiTrackingBulletSpeed;
struct TrackingBullet:bullet_t{
	float angle;
	void JikiMove();
	void TekiMove();
};
//座標を返す
struct pos{
	float x,y;
};

//命クラス
typedef struct{
	float now;
	float max;
}life_t;

//四角形クラス
typedef struct{
	int left;
	int top;
	int right;
	int bottom;
}square_t;

//自機クラス
typedef struct{
	int x,y;
	int speed;
	life_t life;
	int weap;
	int SpjAmount;
	int SpeAmount;
	int SpeOutput;
	bool ahantei; //当たり判定表示フラグ
	bool damage; //ダメージフラグ
}jiki_t;

//敵クラス
typedef struct{
	int x,y;
	life_t life;
	bool damage;
}teki_t;

//タイマー　メソッドはsysmain.cpp
#define MD_TIMER_SHIELD 0
class c_timer{
private:
	int starttime[20];
public:
	void init(int TimerNum);
	bool hasPassed(int TimerNum, unsigned int Sec);
};

typedef struct{
	int bgmvol;
	int sevol;
	bool isFullscreen;
}Save_t;

extern Save_t ConfigData;