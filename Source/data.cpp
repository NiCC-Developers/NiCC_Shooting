#include "common.h"

//グローバル変数
namespace graph{
	int chara[50];
	int hdmaru[10];
	int compterve;
	int textbox;
	int back[10];
}
using namespace graph;

void LoadGraphics(){
	//gui
	textbox=LoadGraph("res\\gui\\wakuwaku.png");
	
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