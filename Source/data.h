namespace graph{
	extern int chara[50];
	extern int hdmaru[10];
	extern int compterve;
	extern int compterve_d;
	extern int en_c[10];
	extern int en_c_d[10];
	extern int ui_main;
	extern int ui_hidariue;
	extern int ui_weap[4];
	extern int ui_spj[7];
	extern int ui_spj_convert[4];
	extern int ui_spj_time;
	extern int ui_weap_meter;
	extern int ui_spe_out[4];
	extern int ui_spe[7];
	extern int ui_spe_meter;

	extern int bg_meteo[3];
	extern int bg_gameover;
	extern int bg_clear;
	extern int textbox;
	extern int pause_back;
	extern int config_back;
	extern int back[10];
	extern int bullet[];
	extern int enm_bullet1;
}

namespace music{
	extern int ms_battle;
	extern int ms_op;
	extern int ms_gameover;
	extern int ms_result;
}

extern void LoadGraphics();
extern void LoadConfigData();
extern void SaveConfigData();
extern void LoadSaveData();
extern void SaveSaveData();
extern void LoadMapData();