/*
stage1_p.h

ヘッダファイル名にアンダースコア＋P (Private)がついているものは、対応した名前のcppファイルだけからincludeするように（プログラマ側で）します。
たとえば、この場合はstage1.cppからstage1_p.hを読み込みますが、他のcppからは参照されません。
逆に、stage1.hはstage1.cppからは読み込まれずにcommon.hからincludeされます。

変数をむやみにグローバル化するのは問題なので、とあるcppでだけ変数を使用したいときにプライベート用ヘッダを使い、そこでexternして使います。
逆に、cppで定義した関数を公開したい場合は _pがついていないヘッダでexternし、common.hに追加して使います。
*/

namespace chara{
	extern bullet_t tb[200]; /*敵弾*/
	extern bullet_t tmb[200]; /*弾移動*/
	extern bullet_t jb[200]; /*自機弾*/
	extern bullet_t jmb[200];
	extern jiki_t jiki;
	extern teki_t boss[20];
}

namespace sys{
	extern square_t framesize;
}