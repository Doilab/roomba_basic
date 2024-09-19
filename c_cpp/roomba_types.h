//------------------------
//データ構造体の定義
//------------------------
#define ROOMBA_TRAJECTORY_MAX 10000 //軌跡のデータ最大値

struct RoombaSensor{
char stat;
int OI_Mode;
int EncL;
int EncR;
char LBumper;
int LBumper_L;
int LBumper_FL;
int LBumper_CL;
int LBumper_CR;
int LBumper_FR;
int LBumper_R;
int Angle;
int Distance;
long TimeNow;//現在時刻
long TimePrev;//前回計測した時刻
int EncL_Prev;//前回エンコーダ値
int EncR_Prev;//前回エンコーダ値
};

struct Odometry{
  double theta;//方位角[rad]
  double x;//座標[m]
  double y;//座標[m]
};


typedef struct
{
  struct RoombaSensor sensor;
  struct Odometry odo;

  double trajectory_x[ROOMBA_TRAJECTORY_MAX];
  double trajectory_y[ROOMBA_TRAJECTORY_MAX];
  int trj_count;

  int roomba_moving_direction=-1;//移動方向を表す変数
  char flag_roomba_moving=0;//移動中のフラグ
  char flag_sensor_ready=0;//センサが使えるかどうかのフラグ

  char sbuf[64];//ルンバ送信バッファ
  char rbuf[512];//ルンバ受信バッファ

    int CommandSpeedL;//左の車輪指令速度
    int CommandSpeedR;//左の車輪指令速度

}RoombaSystem;


typedef struct{
double x;
double y;
}Point2D;
