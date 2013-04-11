#include "common.h"

//グローバル変数
namespace graph{
	int chara[50];
	int hdmaru[10];
	int compterve;
	int textbox;
	int pause_back;
	int config_back;
	int back[10];
}
using namespace graph;


//画像関係ロード
void LoadGraphics(){
	//gui
	textbox=LoadGraph("res\\gui\\wakuwaku.png");
	pause_back=LoadGraph("res\\gui\\pause.png");
	config_back=LoadGraph("res\\gui\\config_back.png");

	//background
	back[0]=LoadGraph("res\\background\\background.png");

	//craft
	hdmaru[0]=LoadGraph("res\\craft\\super_hodakamaru_blue3.png");
	hdmaru[1]=LoadGraph("res\\craft\\super_hodakamaru_blue3.png");
	hdmaru[2]=LoadGraph("res\\craft\\super_hodakamaru_blue3.png");
	hdmaru[3]=LoadGraph("res\\craft\\super_hodakamaru_blue2.png");
	compterve=LoadGraph("res\\craft\\COM_P_TERVE_03.png");

	//character
	chara[0]=LoadGraph("res\\chara\\renkon.png");
	chara[1]=LoadGraph("res\\chara\\renkon2.png");
	chara[2]=LoadGraph("res\\chara\\renkon_fail.png");

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

void SaveConfigData(){
	FILE *fp;
	fp=fopen("setting.cfg","wb");
	fwrite(&ConfigData, sizeof(ConfigData), 1, fp);
	fclose(fp);
}