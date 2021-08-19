#!/usr/bin/env python
# coding: utf-8
#2020.1.14西野君ソースコードQlearn_and_run.pyを参考

import numpy as np
import math
import time
import serial

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
            
    ser.write(bytes([146, R_HB, R_LB, L_HB, L_LB]))

'''センサ値取得関数'''
def GetSensor(ser, p_id, len, sign_flg):
    ser.write(bytes([142, p_id]))
    ser.flushInput()
    data = ser.read(len)
    
    return int.from_bytes(data, "big", signed=sign_flg)

'''各エンコーダ値取得関数'''
def GetEncs(ser):
    EncL = GetSensor(ser, 43, 2, False)
    EncR = GetSensor(ser, 44, 2, False)
    
    return (EncL, EncR)

'''赤外線センサ平均値取得関数'''
def GetBumps(ser):
    BL = GetSensor(ser, 48, 2, False)
    BR = GetSensor(ser, 49, 2, False)
    BumpC = (BR+BL)/2
    
    return BumpC



def main():
    '''シリアル通信用変数'''
    START = bytes([128])
    RESET = bytes([7])
    STOP  = bytes([173])
    SAFE  = bytes([131])
    FULL  = bytes([132])
    #########################################################
    PORT = "COM6"#シリアルポート設定
    #########################################################
    RATE = 115200
    
    print("--- Roomba Control via python ---")
    print("Start Serial Communication")
    '''シリアル通信開始'''
    ser = serial.Serial(PORT, RATE)

    stop_flag=1
    
    while True:
        val=input('Input command char: ')
        print("Input val="+val)
        speed=70
        speed_rot=30
        if stop_flag != 1:
            print("STOP MOTOR")
            stop_flag = 1
            DrivePWM(ser, 0,0)
        elif val=='0':
            print("ROT-R(0)")
            stop_flag = 0
            DrivePWM(ser, speed_rot,-speed_rot)
        elif val=='2':
            print("ROT-L(2)")
            stop_flag = 0
            DrivePWM(ser, -speed_rot,speed_rot)
        elif val=='1':
            print("FWR(1)")
            stop_flag = 0
            DrivePWM(ser, speed,speed)
        elif val=='3':
            print("BACK(3)")
            stop_flag = 0
            DrivePWM(ser, -speed,-speed)
        elif val=='d':
            print("RESET")
            stop_flag = 1
            ser.write(RESET)
            str1 = ser.read(256)
            print(str1)
        elif val=='a':
            print("START")
            stop_flag = 1
            ser.write(START)
        elif val=='g':
            print("SAFE")
            stop_flag = 1
            ser.write(SAFE)
        else:
            print("Input val="+val)
            

main()
