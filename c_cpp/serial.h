//シリアル通信構造体class serial
//serial.cpp,h
//modified 060530
//windows/linuxクロスプラットフォーム
//---------------------------------------------------------------------------
#ifndef serialH
#define serialH
//---------------------------------------------------------------------------

class serial
{
public:
    char flag_opened;//comポートが開かれているかどうか
    char comport[16];//comポート名
    int baudrate;//ボーレートをここに出力

    bool init(char *comport_in,int baudrate);
    bool close(void);
    void purge(void);//WinAPIのPurgeCommを実行する
    int receive(char *buf_ptr,int size);//受け取るバッファの場所とサイズ
    int send(char *buf_ptr,int size);//送るバッファの場所とサイズ
    bool receive2(char *buf_ptr,int size);//ACKを受け取る関数
    unsigned char receive3(char *buf_ptr,int size);//ブロックなしで受信．バイト数を返す

};
//---------------------------------------------------------------------------
#endif
