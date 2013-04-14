#pragma once
extern void TekiMove_1(int *x,int *y);
extern void TekiDraw_1(int x,int y,bool flag);
extern void TekiBullet_1();
extern int TekiHit_1();
extern void ShowNobel_1();
pos searchNearTeki_1(pos bulletPos);

namespace chara{
	extern bullet_t tb[MAX_BULLET_NUM]; /*敵弾*/
	extern bullet_t tmb[MAX_BULLET_NUM]; /*弾移動*/
	extern bullet_t jb[MAX_BULLET_NUM]; /*自機弾*/
	extern bullet_t my_bullet[JIKI_BULLET_KIND][MAX_BULLET_NUM];
	extern TrackingBullet jikiTrackingBullet[MAX_BULLET_NUM];
	extern bullet_t jmb[MAX_BULLET_NUM];
	extern jiki_t jiki;
	extern teki_t boss[20];
}

namespace sys{
	extern square_t framesize;
}