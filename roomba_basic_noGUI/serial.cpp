//---------------------------------------------------------------------------
//シリアル通信構造体class serial
//serial.cpp,h
//要windows.h ->Linux に対応
//modified 060530
//http://www.linux.or.jp/JF/JFdocs/Serial-Programming-HOWTO-3.html
//201101 roomba/Linuxに対応して多少修正

//#define LINUX //Linuxの場合こちらだけ有効
#define WIN32 //Windowsの場合こちらだけ有効

#ifdef LINUX
#undef WIN32
#include <strings.h>//for bzero()
#include <stdlib.h>//for exit
#include <unistd.h>//for read
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#endif

#ifdef WIN32
#undef LINUX
  #include <windows.h>
#endif

#include <stdio.h>
#include "serial.h"

#ifdef LINUX
  int fd, res;
  struct termios oldtio,newtio;
#endif

#ifdef WIN32
    HANDLE hcom;
    DWORD mask;
    COMMTIMEOUTS ctmo;
    OVERLAPPED o;
    COMMPROP cmp;
#endif

//---------------------------------------------------------------------------
bool serial::init(char *comport_in,int baudrate)
{
bool flag=true;

#ifdef WIN32
DCB dcb1;//シリアル通信のパラメータをセットする構造体（ウインドウズ標準）

sprintf(comport,comport_in);

if(flag_opened==1)this->close();
hcom=CreateFile(comport,GENERIC_READ|GENERIC_WRITE,0,
                NULL,OPEN_EXISTING,NULL,NULL);
GetCommState(hcom,&dcb1);//ポートの状態取得

dcb1.BaudRate=baudrate;
//↑Windowsコントロールパネル等で設定して,そのあとここにブレークポイントをおいて，
//変数の値を見るとわかる

dcb1.fParity=1;//0
dcb1.Parity=NOPARITY;//for AI_Motor Roomba
//dcb1.Parity=EVENPARITY;
dcb1.StopBits=ONESTOPBIT;//for AI_Motor Roomba
dcb1.ByteSize=8;//for AI Motor Roomba
dcb1.fNull=FALSE;//こうしておかないとエラーが頻出
//dcb1.EvtChar=STX;
flag=SetCommState(hcom,&dcb1);

    if(flag==true)//イベント設定
    {
     GetCommMask(hcom,&mask);
     //mask=EV_RXCHAR|EV_RXFLAG;a
     mask=EV_RXCHAR;
     flag=SetCommMask(hcom,mask);
    }
    if(flag==true)//タイムアウト設定
    {
        //https://docs.microsoft.com/ja-jp/windows/win32/api/winbase/ns-winbase-commtimeouts?redirectedfrom=MSDN
    GetCommTimeouts(hcom,&ctmo);
	ctmo.ReadIntervalTimeout=3000;//for roomba 210613　文字間の最大タイムアウト？次のバイトが来るまでの最大許される時間
	//ctmo.ReadIntervalTimeout=300;//0;//30;//SICKLMSでは6ms．受信間隔を考慮して適宜設定
	ctmo.ReadTotalTimeoutMultiplier=50;//5000;//=1;// for Roomba 210613　待ち時間計算用一文字当たりの受信時間
	ctmo.ReadTotalTimeoutConstant=0;//1000;//=6;// for Roomba 210613　待ち時間計算用．全体の待ち時間にプラスする定数
    flag=SetCommTimeouts(hcom,&ctmo);
    }

//バッファイニシャライズ
PurgeComm(hcom,PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
//GetCommProperties(hcom,&cmp);
flag_opened=1;
#endif


#ifdef LINUX
 fd = open(comport_in, O_RDWR | O_NOCTTY );
// fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY );
 if (fd <0) {
   perror(comport_in);
   //printf("init() error. %s is not opend. fd=%d\n", comport_in, fd);//debug
   return(false);
 }
 //printf("init() com opened\n");//debug
 flag_opened=1;

 tcgetattr(fd,&oldtio); /* 現在のポート設定を待避 */
 bzero(&newtio, sizeof(newtio));
 newtio=oldtio;//デフォルト値を設定

 tcflag_t baud;
 if(baudrate==9600)baud=B9600;
 if(baudrate==38400)baud=B38400;
 if(baudrate==57600)baud=B57600;
 if(baudrate==115200)baud=B115200;

 // newtio.c_cflag = baud | CRTSCTS | CS8 | CLOCAL | CREAD;
// newtio.c_cflag = baud | IGNPAR | CS8 | CLOCAL | CREAD;
// newtio.c_cflag = baud | CS8 | CREAD;//for roomba 201026
 newtio.c_cflag = baud | CS8 | CLOCAL | CREAD;//for roomba 201026
// newtio.c_iflag = IGNPAR;
// newtio.c_oflag = ;
// newtio.c_oflag = oldtio.c_oflag;

 /* set input mode (non-canonical, no echo,...) */
 newtio.c_lflag = 0;//201026 Roomba
// newtio.c_lflag = oldtio.c_lflag;

 tcsetattr(fd,TCSANOW,&newtio);//201026 新しい設定反映
 flag=true;
#endif

return(flag);
}
//---------------------------------------------------------------------------
void serial::purge(void)
{
//WinAPIのPurgeCommを実行する
  #ifdef WIN32
  PurgeComm(hcom,PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
  #endif

  #ifdef LINUX
  tcflush(fd, TCIFLUSH);
  #endif

}
//---------------------------------------------------------------------------
bool serial::close(void)
{
  if(flag_opened!=1)return false;
  flag_opened=0;

  #ifdef WIN32
  return CloseHandle(hcom);
  #endif

  #ifdef LINUX
  ::close(fd);
  return true;
  #endif
}
//---------------------------------------------------------------------------
int serial::receive(char *buf_ptr,int size)
{
//受信関数（ブロックあり）
//受け取ったバイト長さを返す->良くない．EOFを超えたときにはまる
//ReadFileの返り値がtrueで，byteが0のとき
//->ファイルポインタがEOFを超えてしまった，という意味らしい
unsigned long byte,event;
byte=0;
bool flag=false;

#ifdef WIN32
    while(!flag)
    {
    //文字を受信するまで待つ
    WaitCommEvent(hcom,&event,NULL);
     if(event|EV_RXCHAR)//文字を受信したか？
     {
       if(ReadFile(hcom,buf_ptr,size,&byte,NULL))
       flag=true;
	   break;
     }
     byte=0;//初期化
    }
#endif

#ifdef LINUX
//	tcflush(fd, TCIFLUSH);//受信したがまだ読んでいないデータフラッシュ
// tcsetattr(fd,TCSANOW,&newtio);//受信モードの設定??
   byte = read(fd,buf_ptr,size);   /* 待ち文字入力されたら戻る */
#endif

return byte;
}
//---------------------------------------------------------------------------
bool serial::receive2(char *buf_ptr,int size)
{
//受信関数（ブロックなし）
//ACKを受信するため,ループによるブロックなし．

unsigned long byte,event;
bool flag=false;

#ifdef WIN32
//文字を受信するまで待つ->ACK受信時には待ってはいけない
//WaitCommEvent(hcom,&event,NULL);
       if(ReadFile(hcom,buf_ptr,size,&byte,NULL))
       flag=true;
#endif

return flag;
}
//---------------------------------------------------------------------------
unsigned char serial::receive3(char *buf_ptr,int size)
{
//受信関数（ブロックなし）
//ACKを受信するため,ループによるブロックなし．
//バイト数を返す

unsigned long byte,event;
bool flag=false;

#ifdef WIN32
//文字を受信するまで待つ->ACK受信時には待ってはいけない
//WaitCommEvent(hcom,&event,NULL);
       if(ReadFile(hcom,buf_ptr,size,&byte,NULL))
       flag=true;
#endif

return byte;
}
//---------------------------------------------------------------------------
int serial::send(char *buf_ptr,int size)
{
//バッファの内容を送る
unsigned long byte;
if(flag_opened!=1)
{
  printf("send() error. port Not opend\n");//debug
  printf("flag_opened=%d\n",flag_opened);//debug
  return -1;
}

#ifdef WIN32
    if(WriteFile(hcom,buf_ptr,size,&byte,NULL))
    //↑bufferから読み出し＆hcom(通信ポート)へ書き込み
    {
      return byte;
    }
    else return -1;
#endif

#ifdef LINUX
    byte = write(fd,buf_ptr,size);
    if(byte==0)
      {
	//printf("write error byte 0\n");//debug
	return -1;
      }
    //printf("write byte=%d\n",byte);
    return (byte);
#endif

}
//---------------------------------------------------------------------------

