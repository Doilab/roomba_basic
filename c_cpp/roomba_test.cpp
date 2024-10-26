// あ　UTF-8エンコード
#include <stdio.h>
#include <stdlib.h> //exit()用
#include <math.h>
#include "serial.h"
#include <time.h>
#include <unistd.h> //usleep用
#include "roomba_cmd.h"
#include "roomba_types.h"

double mstime1 = 0; // msec単位での時間計測
double mstime2 = 0;

//--------------
// ☆☆☆☆☆☆☆シリアルポート設定☆☆☆☆☆☆☆☆
// #define SERIAL_PORT "/dev/ttyS16"
#define SERIAL_PORT "\\\\.\\COM5"
//--------------

char message_buf[1024];//メッセージ表示バッファ
char recv_buf[1024];//シリアルポート受信バッファ
char send_buf[1024];//シリアルポート送信バッファ

// 変数用意
RoombaSystem roomba; //ルンバの状態を表すクラス
serial rb_serial; // シリアル通信クラス
char flag_serial_ready; //シリアル通信が準備できているかのフラグ

//--------------------------
// 時間管理
//--------------------------
double get_millisec(void)
{
	double ms_out;

#define CPP11_TIME // Linux, WSL, code::blocks64bit版など
	// #undef CPP11_TIME //code::blocks32bit版など

#ifdef CPP11_TIME
	// Linux C++11用
	struct timespec ts_temp;
	double sec_1, sec_2;
	clock_gettime(CLOCK_REALTIME, &ts_temp);
	sec_1 = ts_temp.tv_sec * 1000;	   // 秒以上msecに直す
	sec_2 = ts_temp.tv_nsec / 1000000; // 秒以下msecに直す
	ms_out = sec_1 + sec_2;
#else
	// clock()関数を使う方法
	clock_t ck1 = clock();
	double sec1 = (double)ck1 / (CLOCKS_PER_SEC);
	ms_out = sec1 * 1000;
#endif

	// printf("get_millisec() %f\n",ms_out);//debug
	return ms_out;
}
void sleep_msec(int millisec_in)
{
	// get_millisec()を使う方法．ビジーループで時間を測る
	double sleep_start = get_millisec();
	double now = sleep_start;
	double dt = 0;

	while (1) // ビジーループ
	{
		now = get_millisec();	// 現在時刻
		dt = now - sleep_start; // 経過時間
		if (dt >= millisec_in)
			break; // チェック
	}

	// usleepを使う方法→あまり精度良くない
	// for(int i=0;i<millisec_in;i++)
	//     usleep(1000);//test
}
//--------------------------
// 変数初期化
//--------------------------
void init()
{
	for (int i = 0; i < 1; i++)
	{
		roomba.roomba_moving_direction = -1; // 移動方向を表す変数
		roomba.flag_roomba_moving = 0;		// 移動中のフラグ
	}

	printf("init()..");
	sleep_msec(3000);
	printf("Done.\n");
}
//--------------------------
// シリアル通信
//--------------------------
void comport_scan()
{
	// COMポートスキャンしてチェック
	serial s1;
	char port_str[128];
	bool res;

	printf("COM port scanning. Available ports: ...\n");
	for (int i = 1; i <= 16; i++)
	{
		sprintf(port_str, "\\\\.\\COM%d", i); // for windows
		// sprintf(port_str,"/dev/ttyS%d",i); //for linux
		res = s1.init(port_str, 115200); // 初期化
		if (res == true)
		{
			printf("[%s]\n", port_str);
			s1.close();
		}
	}
	printf("Done.\n");
}

//--------
char send_command_one(int cmd_in)
{
	char *sbuf = send_buf;
	serial *s = &rb_serial;
	sbuf[0] = (unsigned char)cmd_in; // 1バイト分セット．usigned charに型セット
	s->send(sbuf, 1);				 // コマンド送信

	sprintf(message_buf, "send_command_one(%d) rb[%d]sbuf[0]=(%d)\n", cmd_in, 0, (unsigned char)sbuf[0]);
	printf("%s", message_buf);
	return 1;
}

//--------
char send_drive_command(int motL, int motR)
{
	int byte = 0;
	char *sbuf = send_buf;
	serial *s = &rb_serial;

	sbuf[0] = RB_DRIVE_PWM; // コマンド
	set_drive_command(sbuf + 1, motL, motR);
	byte = 5;
	s->send(sbuf, byte);

	sprintf(message_buf, "send_drive_command() rb[%d]sbuf[]=[%d:%d:%d:%d]\n",
			0,
			(unsigned char)sbuf[0], (unsigned char)sbuf[1], (unsigned char)sbuf[2], (unsigned char)sbuf[3]);
	printf("%s", message_buf);
	return 1;
}
//--------
char send_pwm_motors_command(int main_brush_pwm_in, int side_brush_pwm_in, int vacuum_pwm_in)
{
	int byte = 0;
	char *sbuf = send_buf;
	serial *s = &rb_serial;

	sbuf[0] = RB_PWM_MOTORS;	 // コマンド
	sbuf[1] = main_brush_pwm_in; // メインブラシ-127～+127
	sbuf[2] = side_brush_pwm_in; // サイドブラシ-127～+127
	sbuf[3] = vacuum_pwm_in;	 // 吸引 0～+127
	byte = 4;
	s->send(sbuf, byte);

	sprintf(message_buf, "send_pwm_motors_command() rb[%d]sbuf[]=[%d:%d:%d:%d]\n",
			0,
			(unsigned char)sbuf[0], (unsigned char)sbuf[1], (unsigned char)sbuf[2], (unsigned char)sbuf[3]);
	printf("%s", message_buf);

	return 1;
}

//--------
int send_song_command(int song)
{
	// メロディーセットしたもの送信
	// 返り値バイト数
	int byte;
	char *sbuf = send_buf;
	serial *s = &rb_serial;

	byte = set_songA_command(sbuf, song); // songをセット
	s->send(sbuf, byte);				  // コマンド送信

	return byte;
}
//--------
int send_play_song_command(int song)
{
	int byte = 2;
	serial *s = &rb_serial;
	char *sbuf = send_buf;
	
	sbuf[0] = RB_PLAY;
	sbuf[1] = (unsigned char)song; // 1バイト分セット．usigned charに型セット

	s->send(sbuf, byte); // コマンド送信．

	return byte;
}
//--------------------------
int send_seek_dock_command(void)
{
	int byte = 1;
	serial *s = &rb_serial;
	char *sbuf = send_buf;
	
	sbuf[0] = RB_SEEK_DOCK;
	s->send(sbuf, byte); // コマンド送信．ドック帰還

	return byte;
}
//--------
int send_led_test_command(int color_in, int intensity_in)
{
	int byte = 4;
	serial *s = &rb_serial;
	char *sbuf = send_buf;
	
	sbuf[0] = RB_LEDS;
	sbuf[1] = 0x0;
	// sbuf[1]=0xf;
	sbuf[2] = color_in;		// color
	sbuf[3] = intensity_in; // intensity

	s->send(sbuf, byte); // コマンド送信．再生要求

	return byte;
}
//--------
int send_led_test_command2(void)
{
	int byte = 4;
	send_led_test_command(255, 255);
	return byte;

} //--------------------------
int receive_message(int byte)
{
	// メッセージなど受信
	char *rbuf = recv_buf;
	serial *s = &rb_serial;
	// int res=s->receive(rbuf,byte);
	int res = s->receive3(rbuf, byte);
	if (res < 1)
	{
		printf("Error receive_message()\n");
		return 0;
	}
	printf("receive_message(port-%d)\n", 0);
	printf("---------------\n");
	printf("%s", rbuf);
	printf("\n---------------\n");
	printf("[%d byte]\n", res);
	return 1;
}
//--------------------------
int receive_initial_message(void)
{
	// 初期メッセージ受信
	char rbuf[512];
	for (int i = 0; i < 512; i++)
		rbuf[i] = '\0'; // ゼロを詰める
	int res_byte;
	int pos = 0; // バッファ中の位置
	serial *s = &rb_serial;
	s->purge();
	res_byte = s->receive3(rbuf, 234); // まず234バイト読み込み
	if (res_byte < 1)
	{
		printf("Error receive_message()\n");
		return 0;
	}
	pos = res_byte;
	res_byte = s->receive3(rbuf + pos, 256); // 続きの読み込み（ない場合もあり）
	if (res_byte < 1)
	{
		// printf("Error receive_message()\n");
		// return 0;
		;
	}
	printf("receive_message(port-%d)\n", 0);
	printf("---------------\n");
	printf("%s", rbuf);
	printf("\n---------------\n");
	printf("[%d byte]\n", pos + res_byte);
	return 1;
}
//--------------------------
char get_sensor_1B(int sensor_no)
{
	// 1バイトセンサデータ受信
	if (flag_serial_ready != 1)
		return -1; ////ポート準備ができていなければ処理しない．

	char *sbuf = send_buf;
	char *rbuf = recv_buf;
	serial *s = &rb_serial;

	char db = -1; // databyte

	// センサ情報取得
	// 送信要求
	sbuf[0] = RB_SENSORS;				// コマンド
	sbuf[1] = (unsigned char)sensor_no; // センサ番号
	s->send(sbuf, 2);

	// printf("get_sensor_1B(%d) -- ",sensor_no);
	// 受信
	int res = s->receive(rbuf, 1);
	if (res < 1)
	{
		printf("Error get_sensor_1B()\n");
		return 0;
	}

	char b = rbuf[0];
	// printf("[0x%x]=[%d] \n", b,b);

	db = b;
	return db;
}
//--------------------------
int get_sensor_2B(int sensor_no)
{
	// 2バイト(int)センサデータ受信
	if (flag_serial_ready != 1)
		return -1; ////ポート準備ができていなければ処理しない．

	char *sbuf = send_buf;
	char *rbuf = recv_buf;
	serial *s = &rb_serial;

	int dat = -1; // databyte
	// センサ情報取得
	// 送信要求
	sbuf[0] = RB_SENSORS;				// コマンド
	sbuf[1] = (unsigned char)sensor_no; // センサ番号
	s->send(sbuf, 2);

	// printf("get_sensor_2B(%d) -- ", sensor_no);
	// 受信
	int res = s->receive(rbuf, 2);
	if (res < 2)
	{
		printf("Error get_sensor_2B()\n");
		return 0;
	}

	int hbyte = rbuf[0];
	int lbyte = rbuf[1];
	// printf("H[0x%x]:L[0x%x]  -- val[%d]\n", hbyte, lbyte, val);

	dat = joint_high_low_byte(hbyte, lbyte);

	return dat;
}
//--------------------------
char get_sensors(void)
{
	if (flag_serial_ready != 1)
		return -1; // ポート準備ができていなければ処理しない．

	serial *s = &rb_serial;
	RoombaSensor *rss = &roomba.sensor;

	s->purge(); // シリアル通信のバッファクリア

	rss->stat = get_sensor_1B(58);
	rss->EncL = get_sensor_2B(43);
	rss->EncR = get_sensor_2B(44);
	rss->LBumper = get_sensor_1B(45);
	rss->LBumper_L = get_sensor_2B(46);
	// rss->LBumper_FL=get_sensor_2B(47);
	// rss->LBumper_CL=get_sensor_2B(48);
	// rss->LBumper_CR=get_sensor_2B(49);
	// rss->LBumper_FR = get_sensor_2B(50);
	rss->LBumper_R = get_sensor_2B(51);
	// rss->Angle = get_sensor_2B(20);	   // 値が怪しい
	// rss->Distance = get_sensor_2B(19); // 値が怪しい.ゼロしか出ない

	mstime2 = get_millisec();
	rss->TimeNow = mstime2 - mstime1; // 現在時刻 201101 clock_gettime()使用

	return 1;
}
//--------------------------
void print_sensors(void)
{
	// RoombaSensor *s1=&sensor1;
	RoombaSensor *rss = &roomba.sensor;

	if (flag_serial_ready != 1)
	{
		// ポート準備ができていない場合
		printf("print_sensors() serial port is not ready.\n");
		return;
	}

	printf("\
    stat=%d,\
    encL=%d,\
    encR=%d,\n\
    LBumper=0x%x,\
    LBumper_L=%d,\
    LBumper_R=%d,\
    Time=%.0f,\
    \n",
		   rss->stat,
		   rss->EncL, rss->EncR,
		   rss->LBumper, rss->LBumper_L, rss->LBumper_R,
		   rss->TimeNow);

	return;
}
//--------------------------
//-------------------------------------
void drive_tires(int dir_in)
{
	
	RoombaSystem *rb = &roomba;

	int speed = 70;
	int speed_rot = 40;

	if (rb->flag_roomba_moving == 1) // 移動中のボタン入力→移動キャンセル
	{
		printf("STOP\n");
		rb->CommandSpeedL = 0;
		rb->CommandSpeedR = 0;
		rb->flag_roomba_moving = 0;

		send_drive_command(0, 0);

		return;
	}

	if (dir_in == 1)
	{
		printf("FWD\n");
		rb->CommandSpeedL = speed;
		rb->CommandSpeedR = speed;
		rb->flag_roomba_moving = 1;
	}
	else if (dir_in == 3)
	{
		printf("BACK\n");
		rb->CommandSpeedL = -speed;
		rb->CommandSpeedR = -speed;
		rb->flag_roomba_moving = 1;
	}
	else if (dir_in == 0)
	{
		printf("ROT-CW\n");
		rb->CommandSpeedL = -speed_rot;
		rb->CommandSpeedR = speed_rot;
		rb->flag_roomba_moving = 1;
	}
	else if (dir_in == 2)
	{
		printf("ROT-CCW\n");
		rb->CommandSpeedL = speed_rot;
		rb->CommandSpeedR = -speed_rot;
		rb->flag_roomba_moving = 1;
	}
	else
	{
		printf("STOP\n");
		rb->CommandSpeedL = 0;
		rb->CommandSpeedR = 0;
		rb->flag_roomba_moving = 0;
	}
	send_drive_command(rb->CommandSpeedL, rb->CommandSpeedR);
}

//-----------------------
void print_keys(void)
{
	printf("---------------------\n");
	printf("a: START\n");
	printf("s: STOP\n");
	printf("d: RESET\n");
	printf("g: SAFE\n");
	printf("f: FULL\n");
	printf("c: clear buffer\n");
	printf("\n");
	printf("0: turn right\n");
	printf("1: go forward\n");
	printf("2: turn left\n");
	printf("3: go backward\n");
	printf("\n");
	printf("z: get sensor info\n");
	printf("x: play song\n");
	printf("i: init sensor\n");
	printf("v: vacuum on\n");
	printf("b: vacuum off\n");
	printf("w: dock\n");
	printf("---------------------\n");
}

//-----------------------
void keyf(unsigned char key, int x, int y) // 一般キー入力
{
	// キー入力で指定された処理実行

	//	printf("key[%c],x[%d],y[%d]\n",key,x,y);
	printf("key[%c]\n", key);

	// int port=current_control_port;
	RoombaSystem *rb = &roomba;

	switch (key)
	{
	case 'a':
	{
		send_command_one(RB_START);
		break;
	}
	case 's':
	{
		send_command_one(RB_STOP);
		break;
	}
	case 'd':
	{
		send_command_one(RB_RESET);
		// receive_message(port, 256);//初期化メッセージ受信
		receive_initial_message(); // 初期化メッセージ受信
		break;
	}
	case 'c':
	{
		receive_initial_message(); // 初期化メッセージ受信　受信バッファクリア
		break;
	}
	case 'f':
	{
		send_command_one(RB_FULL);
		break;
	}
	case 'g':
	{
		send_command_one(RB_SAFE);
		break;
	}
	case 'z':
	{
		 get_sensors(); // read sensors
		 print_sensors();
		break;
	}
	case 'x':
	{
		send_song_command(0);
		send_play_song_command(0);
		break;
	}
	case 'i':
	{
		init();
		break;
	}
	case 'v':
	{
		printf("Vacuum ON\n");
		send_pwm_motors_command(0, 100, 100); // 吸引ON
		break;
	}
	case 'b':
	{
		printf("Vacuum OFF\n");
		send_pwm_motors_command(0, 0, 0); // 吸引OFF
		break;
	}
	case 'w':
	{
		printf("--- Go Back to DOCK! --- \n");
		send_seek_dock_command(); // ドックに戻る?
		break;
	}
	case 'l':
	{
		// LED ON
		send_led_test_command(255, 255);
		break;
		;
	}
	case 'o':
	{
		// LED OFF
		send_led_test_command(255, 0);
		break;
		;
	}
	case '0':
	{
		drive_tires(0);
		break;
	}
	case '1':
	{
		drive_tires(1);
		break;
	}
	case '2':
	{
		drive_tires(2);
		break;
	}
	case '3':
	{
		drive_tires(3);
		break;
	}
	case 'q':
	case 'Q':
	case '\033': /* '\033' は ESC の ASCII コード */
	{
		printf("Exit\n");
		exit(0);
		break;
	}
	case 32: // 32がスペースを表す
	{
		printf("SPACE\n");
		rb->roomba_moving_direction = -1;
		rb->flag_roomba_moving = 0;
		drive_tires(-1); // stop
		break;
	}
	default:
	{
		// print_keys();
		printf("\n");
		break;
	}
	}
}
//-----------------------
void key_input(void)
{
	int key;
	while (1)
	{
		printf("keyf() input: ");
		key = getchar();
		printf("[%c]\n", key);
		keyf(key, 0, 0);
	}
}

//-----------------------
int main(int argc, char **argv)
{
	//int id;

	// シリアルポートスキャン
	comport_scan();

	// シリアルポート初期設定
	bool res;
	flag_serial_ready = 0;

	res = rb_serial.init(SERIAL_PORT, 115200); // シリアルポート初期化
	rb_serial.purge();						   // シリアルポートバッファクリア
	if (res != true)
	{
		printf("Port Open Error#0 [%s]\n", SERIAL_PORT);
	}
	else
	{
		printf("Port[%s] Ready.\n", SERIAL_PORT);
		flag_serial_ready = 1;
	}

	init();					  // 変数初期化
	mstime1 = get_millisec(); // 時間計測開始

	// キーボード割り当て表示
	print_keys();

	// キーボード入力受付
	key_input(); // for NoGUI

	return 0;
}
