//Roombaとの通信コマンド定義
//符号なしにするためにはUをつける

#define RB_OFF
#define RB_PASSIVE
#define RB_SAFE 131U
#define RB_FULL 132U

#define RB_START 128U
#define RB_RESET 7U
#define RB_STOP 173U

#define RB_POWER 133U //パワーダウン
#define RB_DRIVE 137U //速度，旋回半径指定駆動．4バイトデータ必要
#define RB_DRIVE_PWM 146U //4バイトデータ必要
#define RB_MOTORS 138U //ブラシ・吸引などの方向指定．1バイトデータ必要
#define RB_PWM_MOTORS 144U //ブラシ・吸引などの強さ指定．1バイトデータ必要

#define RB_SENSORS 142U //センサデータ要求．1バイトデータ必要

#define RB_LIGHT_BUMPER 45U //光バンパーが反応したかどうか
#define RB_LIGHT_BUMPER_L_SIG 46U //光バンパー左
#define RB_LIGHT_BUMPER_FL_SIG 47U //光バンパー左前
#define RB_LIGHT_BUMPER_CL_SIG 48U //光バンパー左中
#define RB_LIGHT_BUMPER_CR_SIG 49U //光バンパー右中
#define RB_LIGHT_BUMPER_FR_SIG 50U //光バンパー右前
#define RB_LIGHT_BUMPER_R_SIG 51U //光バンパー右

#define RB_LEFT_ENC 43U //左エンコーダカウント
#define RB_RIGHT_ENC 44U //右エンコーダカウント

#define RB_SONG 140U //メロディ記憶．
#define RB_PLAY 141U //メロディ再生．1バイトデータ必要

#define RB_OI_MODE 35U //ルンバのモードを返す

#define RB_SEEK_DOCK 143U //ドックを探す

#define RB_LEDS 139U //LED制御

//---------------------------
//コマンドセット関数
//配列にコマンドをセットする
//---------------------------

//--------------------------
int joint_high_low_byte(int hbyte, int lbyte)
{
	//上位バイトと下位バイトの結合
	int hb,lb,val;
	hb=(0x00ff&hbyte);
	val=(hbyte<<8);
	val|=(0x00ff&lbyte);
    //printf("H[0x%x]:L[0x%x]  -- val[%d]\n", hbyte, lbyte, val);
	return val;
}
//---------------------------------------
int set_songA_command(char *buf_out, int song)
{
    //メロディーバッファにセット
    //返り値バイト数
	int byte;

	buf_out[0]=RB_SONG;
	buf_out[1]=(unsigned char)song;//1バイト分セット．usigned charに型セット
	int length=11;//曲の長さ
	buf_out[2]=length;//曲の長さ

	buf_out[3]=90;//音程
	buf_out[4]=16;//音の長さ
	buf_out[5]=86;//音程
	buf_out[6]=16;//音の長さ
	buf_out[7]=81;//音程
	buf_out[8]=16;//音の長さ
	buf_out[9]=86;//音程
	buf_out[10]=16;//音の長さ

	buf_out[11]=88;//音程
	buf_out[12]=16;//音の長さ
	buf_out[13]=93;//音程
	buf_out[14]=48;//音の長さ

	buf_out[15]=88;//音程
	buf_out[16]=16;//音の長さ
	buf_out[17]=90;//音程
	buf_out[18]=16;//音の長さ
	buf_out[19]=88;//音程
	buf_out[20]=16;//音の長さ
	buf_out[21]=81;//音程
	buf_out[22]=16;//音の長さ
	buf_out[23]=86;//音程
	buf_out[24]=48;//音の長さ

	byte=3+length*2;
	//rb_s1.send(buf_out,byte);//コマンド送信

	return byte;

}

//--------------------------------------------------
int set_songB_command(char *buf_out, int song)
{
    //メロディーバッファにセット
    //返り値バイト数
	int byte;

	buf_out[0]=RB_SONG;
	buf_out[1]=(unsigned char)song;//1バイト分セット．usigned charに型セット
	int length=17;//曲の長さ
	buf_out[2]=length;//曲の長さ
	int L=24;//1拍の長さ


	buf_out[3]=64;//音程
	buf_out[4]=L*3;//音の長さ
	buf_out[5]=67;//音程
	buf_out[6]=L*2;//音の長さ
	buf_out[7]=69;//音程
	buf_out[8]=L*1;//音の長さ
	buf_out[9]=69;//音程
	buf_out[10]=L*6;//音の長さ

	buf_out[11]=65;//音程
	buf_out[12]=L*3;//音の長さ
	buf_out[13]=69;//音程
	buf_out[14]=L*2;//音の長さ
	buf_out[15]=71;//音程
	buf_out[16]=L*1;//音の長さ
	buf_out[17]=71;//音程
	buf_out[18]=L*6;//音の長さ

	buf_out[19]=74;//音程
	buf_out[20]=L*3;//音の長さ
	buf_out[21]=72;//音程
	buf_out[22]=L*2;//音の長さ
	buf_out[23]=64;//音程
	buf_out[24]=L*1;//音の長さ

	buf_out[25]=67;//音程
	buf_out[26]=L*3;//音の長さ
	buf_out[27]=65;//音程
	buf_out[28]=L*2;//音の長さ
	buf_out[29]=64;//音程
	buf_out[30]=L*1;//音の長さ

	buf_out[31]=64;//音程
	buf_out[32]=L*3;//音の長さ
	buf_out[33]=62;//音程
	buf_out[34]=L*3;//音の長さ
	buf_out[35]=60;//音程
	buf_out[36]=L*6;//音の長さ

	byte=3+length*2;
	//rb_s1.send(buf_out,byte);//コマンド送信

	return byte;

}

//--------------------------------------------------
int set_christmas_song_command(char *buf_out, int song)
{
    //メロディーバッファにセット
    //返り値バイト数
	int byte;

	buf_out[0]=RB_SONG;
	buf_out[1]=(unsigned char)song;//1バイト分セット．usigned charに型セット
	int length=14;//曲の長さ
	buf_out[2]=length;//曲の長さ

	int L=2;//1拍の長さ
	buf_out[3]=86;//音程
	buf_out[4]=32*L;//音の長さ
	buf_out[5]=86;//音程
	buf_out[6]=16*L;//音の長さ
	buf_out[7]=89;//音程
	buf_out[8]=24*L;//音の長さ
	buf_out[9]=86;//音程
	buf_out[10]=8*L;//音の長さ
	buf_out[11]=83;//音程
	buf_out[12]=16*L;//音の長さ

	buf_out[13]=84;//音程
	buf_out[14]=48*L;//音の長さ
	buf_out[15]=88;//音程
	buf_out[16]=48*L;//音の長さ

	buf_out[17]=84;//音程
	buf_out[18]=16*L;//音の長さ
	buf_out[19]=79;//音程
	buf_out[20]=16*L;//音の長さ
	buf_out[21]=76;//音程
	buf_out[22]=16*L;//音の長さ
	buf_out[23]=79;//音程
	buf_out[24]=24*L;//音の長さ
	buf_out[25]=77;//音程
	buf_out[26]=8*L;//音の長さ
	buf_out[27]=74;//音程
	buf_out[28]=16*L;//音の長さ

	buf_out[29]=72;//音程
	buf_out[30]=96*L;//音の長さ


	byte=3+length*2;

	return byte;

}

//--------------------------------------------------
char set_drive_command(char *buf_out, int Lpwm, int Rpwm)
{
	int LHbyte=Lpwm&0xff00;
	LHbyte=(LHbyte>>8);
	int LLbyte=Lpwm&0x00ff;
	int RHbyte=Rpwm&0xff00;
	RHbyte=(RHbyte>>8);
	int RLbyte=Rpwm&0x00ff;

	buf_out[0]=(unsigned char)LHbyte;
	buf_out[1]=(unsigned char)LLbyte;
	buf_out[2]=(unsigned char)RHbyte;
	buf_out[3]=(unsigned char)RLbyte;

	return 1;
}
//--------------------------------------------------
