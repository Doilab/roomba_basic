'''シリアル通信用設定変数'''
RB_LEFT_ENC = 43 #左エンコーダカウント
RB_RIGHT_ENC = 44 #//右エンコーダカウント

RB_SONG  = 140 #//メロディ記憶．
RB_PLAY  = 141 #//メロディ再生．1バイトデータ必要
RB_OI_MODE  = 35 #//ルンバのモードを返す
RB_LEDS  = 139 #//LED制御
RB_DRIVE_PWM  = 146 #//PWMでモータ駆動
RB_MOTORS = 138 #//ブラシ・吸引などの方向指定．1バイトデータ必要
RB_PWM_MOTORS = 144 #//ブラシ・吸引などの強さ指定．1バイトデータ必要

RB_VOLTAGE = 22 #バッテリー電圧
RB_CURRENT = 23 #バッテリー電圧
RB_SENSORS = 142 #センサ情報取得

RB_RESET = 7 #リセット
RB_START = 128 #スタート
RB_STOP  = 173
RB_SAFE  = 131
RB_FULL  = 132

'''モータ入力用バイト計算関数'''
def _CalcHLByte(n):
    HB = n & 0xff00
    HB >>= 8
    LB = n & 0x00ff
    
    return (HB, LB)

'''長いメロディ再生関数'''
def PlaySongLong(ser, n):
    melody1=[11,   
    90,16, 86,16, 81,16, 86,16, 88,16, 93,48,     
    88,16, 90,16, 88,16, 81,16, 86,48]#長いメロディ（ファミマ）

    melody2A=[58,
    79,16, 79,32, 81,16, 79,16, 76,16, 79,16, 0,16,
    79,16, 79,32, 81,16, 79,16, 76,16, 79,16, 0,16,
    72,16, 72,16, 72,16, 74,16, 76,48, 72,16, 
    76,48, 79,16, 79,64, 
    72,16, 72,16, 72,16, 74,16, 76,64, 
    72,16, 72,16, 72,16, 74,16, 76,64,
    74,16, 74,16, 74,16, 72,16, 74,32, 76,32,
    79,32, 77,32, 76,32, 74,32,
    79,16, 79,32, 81,16, 79,16, 76,16, 79,16, 0,16,
    79,16, 79,32, 81,16, 79,16, 76,16, 74,16, 0,16,
    72,96
    ]
    ser.write(bytes([RB_SONG, n]))#songセット準備
    ser.write(bytes(melody2A))#songセット
    ser.write(bytes([RB_PLAY, n]))#song 再生
    #ser.write(bytes([RB_SONG, n]))#songセット準備
    #ser.write(bytes(melody2B))#songセット
    #ser.write(bytes([RB_PLAY, n]))#song 再生

'''掃除などPWM指令関数'''
def PWMMotors(ser, MainBPwm, SubBPwm, VacPwm):
    ser.write(bytes([RB_PWM_MOTORS, MainBPwm, SubBPwm, VacPwm]))
    #ser.write(bytes([144, 0,0,50]))
