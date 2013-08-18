#include "includer.h"

//グローバル変数
namespace graph{
	int chara[50];
	int hdmaru[10];
	int compterve;
	int compterve_d;
	int en_c[10];
	int en_c_d[10];
	int ui_main;
	int ui_hidariue;
	int ui_weap[4];
	int ui_spj[7];
	int ui_spj_convert[4];
	int ui_spj_time;
	int ui_weap_meter;
	int ui_spe_out[4];
	int ui_spe[7];
	int ui_spe_meter;
	int bg_meteo[3];
	int bg_gameover;
	int bg_clear;
	int textbox;
	int pause_back;
	int config_back;
	int back[10];
	int bullet[10];
	int enm_bullet1;
}
namespace music{
	int ms_battle;
	int ms_op;
	int ms_gameover;
	int ms_result;
}

using namespace graph;
using namespace music;



//画像関係ロード
void LoadGraphics(){
	//gui
	ui_main=LoadGraph("res\\gui\\ui_base_n.png");
	ui_hidariue=LoadGraph("res\\gui\\ui_hidariue.png");
	ui_weap[1]=LoadGraph("res\\gui\\ui_weap_1.png");
	ui_weap[2]=LoadGraph("res\\gui\\ui_weap_2.png");
	ui_weap[3]=LoadGraph("res\\gui\\ui_weap_3.png");
	ui_spj[0]=LoadGraph("res\\gui\\ui_SPJ0.png");
	ui_spj[1]=LoadGraph("res\\gui\\ui_SPJ1.png");
	ui_spj[2]=LoadGraph("res\\gui\\ui_SPJ2.png");
	ui_spj[3]=LoadGraph("res\\gui\\ui_SPJ3.png");
	ui_spj[4]=LoadGraph("res\\gui\\ui_SPJ4.png");
	ui_spj[5]=LoadGraph("res\\gui\\ui_SPJ5.png");
	ui_spj[6]=LoadGraph("res\\gui\\ui_SPJ6.png");
	ui_spj_convert[0]=LoadGraph("res\\gui\\ui_spj_convert_1.png");
	ui_spj_convert[1]=LoadGraph("res\\gui\\ui_spj_convert_2.png");
	ui_spj_convert[2]=LoadGraph("res\\gui\\ui_spj_convert_3.png");
	ui_spj_convert[3]=LoadGraph("res\\gui\\ui_spj_convert_4.png");
	ui_spj_time=LoadGraph("res\\gui\\ui_spj_time.png");
	ui_weap_meter=LoadGraph("res\\gui\\ui_weap_meter.png");
	ui_spe_out[1]=LoadGraph("res\\gui\\ui_spe_out_shield.png");
	ui_spe_out[0]=LoadGraph("res\\gui\\ui_spe_out_weap.png");
	ui_spe[0]=LoadGraph("res\\gui\\ui_spe_0.png");
	ui_spe[1]=LoadGraph("res\\gui\\ui_spe_1.png");
	ui_spe[2]=LoadGraph("res\\gui\\ui_spe_2.png");
	ui_spe[3]=LoadGraph("res\\gui\\ui_spe_3.png");
	ui_spe[4]=LoadGraph("res\\gui\\ui_spe_4.png");
	ui_spe[5]=LoadGraph("res\\gui\\ui_spe_5.png");
	ui_spe[6]=LoadGraph("res\\gui\\ui_spe_6.png");
	ui_spe_meter=LoadGraph("res\\gui\\ui_spe_meter.png");

	textbox=LoadGraph("res\\gui\\textbox.png");
	pause_back=LoadGraph("res\\gui\\pause.png");
	config_back=LoadGraph("res\\gui\\config_back.png");

	//background
	back[0]=LoadGraph("res\\background\\bg_star.png");
	back[1]=LoadGraph("res\\background\\bg_star_front.png");
	bg_meteo[0]=LoadGraph("res\\background\\bg_meteo_1.png");
	bg_meteo[1]=LoadGraph("res\\background\\bg_meteo_2.png");
	bg_meteo[2]=LoadGraph("res\\background\\bg_meteo_3.png");
	bg_gameover=LoadGraph("res\\background\\gameover.png");
	bg_clear=LoadGraph("res\\background\\clear.png");

	//craft
	hdmaru[0]=LoadGraph("res\\craft\\super_hodakamaru_blue3.png");
	hdmaru[1]=LoadGraph("res\\craft\\super_hodakamaru_blue3.png");
	hdmaru[2]=LoadGraph("res\\craft\\super_hodakamaru_blue3.png");
	hdmaru[3]=LoadGraph("res\\craft\\super_hodakamaru_blue2.png");
	compterve=LoadGraph("res\\craft\\COM_P_TERVE_03.png");
	compterve_d=LoadGraph("res\\craft\\COM_P_TERVE_03_d.png");
	en_c[1]=LoadGraph("res\\craft\\en_c2.png");
	en_c_d[1]=LoadGraph("res\\craft\\en_c2_d.png");
	//character
	graph::chara[0]=LoadGraph("res\\chara\\renkon.png");
	graph::chara[1]=LoadGraph("res\\chara\\renkon2.png");
	graph::chara[2]=LoadGraph("res\\chara\\renkon_fail.png");

	//bullet
	bullet[0]=LoadGraph("res\\bullet\\ballet1.png");
	bullet[1]=LoadGraph("res\\bullet\\ballet2.png");
	enm_bullet1=LoadGraph("res\\bullet\\enm_bullet1.png");
	//仮のデータ
	bullet[2]=bullet[1];

	//音楽ロード
	ms_battle=LoadSoundMem("res\\bgm\\battle.wav");
	ms_op=LoadSoundMem("res\\bgm\\op.mp3");
	ms_gameover=LoadSoundMem("res\\bgm\\gameover.wav");
	ms_result=LoadSoundMem("res\\bgm\\result.wav");
}

//セーブデータ
void LoadConfigData(){
	FILE *fp;

	if((fp = fopen("setting.cfg","rb")) == NULL) {
		fp = fopen("setting.cfg","wb");
		fwrite(&ConfigData, sizeof(ConfigData), 1, fp);
		fclose(fp);
	} else {
		fread(&ConfigData, sizeof(ConfigData), 1, fp);
		fclose(fp);
	}

}

void LoadSaveData(){
	FILE *fp;

	if((fp = fopen("SaveData.dat","rb")) == NULL) {
		fp = fopen("SaveData.dat","wb");
		fwrite(&HighScore, sizeof(HighScore), 1, fp);
		fclose(fp);
	} else {
		fread(&HighScore, sizeof(HighScore), 1, fp);
		fclose(fp);
	}

}

void SaveSaveData(){
	FILE *fp;
	fp=fopen("SaveData.dat","wb");
	fwrite(&HighScore, sizeof(HighScore), 1, fp);
	fclose(fp);
}



//マップデータ
void LoadMapData(){
		FILE *fp;
	if((fopen_s(&fp,"Enemy.map","rb"))!=0){
		fopen_s(&fp,"Enemy.map","wb");
		fwrite(&TimeLine, sizeof(TimeLine), 1, fp);
		fclose(fp);
	}else{
		fread(&TimeLine, sizeof(TimeLine), 1, fp);
		fclose(fp);
	}
}

void SaveConfigData(){
	FILE *fp;
	fp=fopen("setting.cfg","wb");
	fwrite(&ConfigData, sizeof(ConfigData), 1, fp);
	fclose(fp);
}
