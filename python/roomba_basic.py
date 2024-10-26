#!/usr/bin/env python
# coding: utf-8
#2020.1.14西野君ソースコードQlearn_and_run.pyを参考
#2024.9.20 update

import numpy as np
import math
import time
import serial
import sys
sys.path.append("./")
import roomba_def as rb

#########################################################
#シリアルポート設定
RB_PORT = "COM5"#ポート
RB_RATE = 115200#ビットレート
#########################################################



'''モータへのPWM信号入力関数'''
def DrivePWM(ser, L_PWM, R_PWM):
    L_HB, L_LB = rb._CalcHLByte(L_PWM)
    R_HB, R_LB = rb._CalcHLByte(R_PWM)
            
    ser.write(bytes([rb.RB_DRIVE_PWM, R_HB, R_LB, L_HB, L_LB]))


'''センサ値取得関数'''
def GetSensor(ser, p_id, len, sign_flg):
    ser.write(bytes([rb.RB_SENSORS, p_id]))
    ser.flushInput()
    data = ser.read(len)
    
    return int.from_bytes(data, "big", signed=sign_flg)

'''各エンコーダ値取得関数'''
def GetEncs(ser):
    EncL = GetSensor(ser, rb.RB_LEFT_ENC, 2, False)
    EncR = GetSensor(ser, rb.RB_RIGHT_ENC, 2, False)
    
    return (EncL, EncR)

'''モード取得関数'''
def GetOIMode(ser):
    oimode = GetSensor(ser, rb.RB_OI_MODE, 1, False)
    
    return (oimode)

'''モード変更関数'''
def SetOIMode(ser, mode_in):
    oimode1 = GetOIMode(ser)
    ser.write(bytes([mode_in]))
    oimode2 = GetOIMode(ser)
    print("OIMode:"+str(oimode1)+"->"+str(oimode2))    
    return (oimode2)


'''メロディ再生関数'''
def PlaySong(ser, n):
    ser.write(bytes([rb.RB_SONG, n]))#songセット準備
    
    ser.write(bytes([3]))#song セット　音の数
    ser.write(bytes([90, 16]))#songセット　音程と音の長さ
    ser.write(bytes([86, 16]))#songセット　音程と音の長さ
    ser.write(bytes([81,16]))#songセット　音程と音の長さ
   
    ser.write(bytes([rb.RB_PLAY, n]))#song 再生



def main():
    '''メイン制御関数'''
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
            print("SONG SET and PLAY")#song
            PlaySong(ser, 0)
            #rb.PlaySongLong(ser,0)

        elif val=='v':
            print("Vacuum ON")
            rb.PWMMotors(ser,0,100,100)

        elif val=='b':
            print("Vacuum OFF")
            rb.PWMMotors(ser,0,0,0)

        elif val=='d':
            print("RESET")
            stop_flag = 1
            ser.write(bytes([rb.RB_RESET]))
            str1 = ser.read(234)
            #str1 = ser.read(512)
            print(str1)

        elif val=='a':
            print("START")
            stop_flag = 1
            SetOIMode(ser,rb.RB_START)

        elif val=='g':
            print("SAFE")
            stop_flag = 1
            SetOIMode(ser,rb.RB_SAFE)

        elif val=='f':
            print("FULL")
            stop_flag = 1
            SetOIMode(ser,rb.RB_FULL)

        elif val=='z':
            print("SENSOR")
            el,er = GetEncs(ser)
            oimode = GetOIMode(ser)
            vol = GetSensor(ser, rb.RB_VOLTAGE, 2, False)
            cur = GetSensor(ser, rb.RB_CURRENT, 2, False)
            
            print("Enc L:"+str(el)+" R:"+str(er))
            print("OIMode:"+str(oimode))
            print("Votage/Current ="+str(vol)+"[mV]/"+str(cur)+"[mA]")

        elif val=='o':
            print("TEST MODE")
            ########## ここに何か追加する #####################
            DrivePWM(ser, 0,0)#左，右のスピード設定
            time.sleep(0.5) #待ち時間設定
            DrivePWM(ser, 0,0)#左，右のスピード設定
        
            
        elif val=='q': #プログラム終了
            print("QUIT")
            quit()
 

        else:
            print("Input val="+val)
            

main()
