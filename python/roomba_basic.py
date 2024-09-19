#!/usr/bin/env python
# coding: utf-8
#2020.1.14西野君ソースコードQlearn_and_run.pyを参考
#2024.9.19 update

import numpy as np
import math
import time
import serial

#########################################################
RB_PORT = "COM5"#シリアルポート設定
#########################################################

'''シリアル通信用コマンド変数'''
RB_LEFT_ENC = 43 #左エンコーダカウント
RB_RIGHT_ENC = 44 #//右エンコーダカウント

RB_SONG  = 140 #//メロディ記憶．
RB_PLAY  = 141 #//メロディ再生．1バイトデータ必要
RB_OI_MODE  = 35 #//ルンバのモードを返す
RB_LEDS  = 139 #//LED制御
RB_DRIVE_PWM  = 146 #//PWMでモータ駆動

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

'''モータへのPWM信号入力関数'''
def DrivePWM(ser, L_PWM, R_PWM):
    L_HB, L_LB = _CalcHLByte(L_PWM)
    R_HB, R_LB = _CalcHLByte(R_PWM)
            
    ser.write(bytes([RB_DRIVE_PWM, R_HB, R_LB, L_HB, L_LB]))

'''センサ値取得関数'''
def GetSensor(ser, p_id, len, sign_flg):
    ser.write(bytes([RB_SENSORS, p_id]))
    ser.flushInput()
    data = ser.read(len)
    
    return int.from_bytes(data, "big", signed=sign_flg)

'''各エンコーダ値取得関数'''
def GetEncs(ser):
    EncL = GetSensor(ser, RB_LEFT_ENC, 2, False)
    EncR = GetSensor(ser, RB_RIGHT_ENC, 2, False)
    
    return (EncL, EncR)

'''モード取得関数'''
def GetOIMode(ser):
    oimode = GetSensor(ser, RB_OI_MODE, 1, False)
    
    return (oimode)

'''モード変更関数'''
def SetOIMode(ser, mode_in):
    oimode1 = GetOIMode(ser)
    ser.write(bytes(mode_in))
    oimode2 = GetOIMode(ser)
    print("OIMode:"+str(oimode1)+"->"+str(oimode2))    
    return (oimode2)


'''メロディ再生関数'''
def PlaySong(ser, n):
    ser.write(bytes([RB_SONG, n]))#songセット準備
    
    ser.write(bytes([3]))#song セット　音の数
    ser.write(bytes([90, 16]))#songセット　音程と音の長さ
    ser.write(bytes([86, 16]))#songセット　音程と音の長さ
    ser.write(bytes([81,16]))#songセット　音程と音の長さ
   
    ser.write(bytes([RB_PLAY, n]))#song 再生


def PlaySongLong(ser, n):
    melody1=[11,   
    90,16, 86,16, 81,16, 86,16, 88,16, 93,48,     
    88,16, 90,16, 88,16, 81,16, 86,48]#長いメロディ
    
    ser.write(bytes([RB_SONG, n]))#songセット準備
    ser.write(bytes(melody1))#songセット
    ser.write(bytes([RB_PLAY, n]))#song 再生


def main():
    '''シリアル通信用変数'''
    #RB_START = bytes([128])
    #RB_RESET = bytes([7])
    RB_STOP  = bytes([173])
    RB_SAFE  = bytes([131])
    RB_FULL  = bytes([132])
    
    RB_RATE = 115200
    
    print("--- Roomba Control via python ---")
    #print("Start Serial Communication")
    '''シリアル通信開始'''
    ser = serial.Serial(RB_PORT, RB_RATE, timeout=10)

    stop_flag=1
    
    while True:
        val=input('Input command char: ')
        #print("Input val="+val)
        speed=70
        speed_rot=50
        if stop_flag != 1:
            print("STOP MOTOR")
            stop_flag = 1
            DrivePWM(ser, 0,0)#左，右のスピード
        elif val=='0':
            print("ROT-R(0)")
            stop_flag = 0
            DrivePWM(ser, 50,-50)#左，右のスピード
        elif val=='2':
            print("ROT-L(2)")
            stop_flag = 0
            DrivePWM(ser, -50, 50)#左，右のスピード
        elif val=='1':
            print("FWR(1)")
            stop_flag = 0
            DrivePWM(ser, 70,70)#左，右のスピード
        elif val=='3':
            print("BACK(3)")
            stop_flag = 0
            DrivePWM(ser, -70,-70)#左，右のスピード
        elif val=='x':
            print("SONG SET and PLAY")
            PlaySong(ser, 0)
        elif val=='d':
            print("RESET")
            stop_flag = 1
            ser.write(bytes([RB_RESET]))
            str1 = ser.read(234)
            print(str1)
        elif val=='a':
            print("START")
            stop_flag = 1
            SetOIMode(ser,RB_START)
        elif val=='g':
            print("SAFE")
            stop_flag = 1
            SetOIMode(ser,RB_SAFE)
        elif val=='f':
            print("FULL")
            stop_flag = 1
            SetOIMode(ser,RB_FULL)
        elif val=='z':
            print("SENSOR")
            el,er = GetEncs(ser)
            oimode = GetOIMode(ser)
            vol = GetSensor(ser, RB_VOLTAGE, 2, False)
            cur = GetSensor(ser, RB_CURRENT, 2, False)
            
            print("Enc L:"+str(el)+" R:"+str(er))
            print("OIMode:"+str(oimode))
            print("Votage/Current ="+str(vol)+"[mV]/"+str(cur)+"[mA]")


        elif val=='o':
            print("TEST MODE")
            #ここに何か追加する
            
        elif val=='q':
            print("QUIT")
            quit()
 
        else:
            print("Input val="+val)
            

main()
