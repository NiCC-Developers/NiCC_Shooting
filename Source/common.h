/*
common.h

共通ヘッダファイル
*/

#pragma once

#include <DxLib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "main.h"
#include "sysmain.h"
#include "ui.h"
#include "text.h"
#include "data.h"

#include "stage1.h"

#define PI 3.141592

//弾クラス
typedef struct{
	float x;
	float y;
	bool avail;
}bullet_t;

//命クラス
typedef struct{
	int now;
	int max;
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
	bool ahantei; //当たり判定表示フラグ
	bool damage; //ダメージフラグ
}jiki_t;

//敵クラス
typedef struct{
	int x,y;
	life_t life;
	bool damage;
}teki_t;

