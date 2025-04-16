#!/usr/bin/env python

#
# Copyright (c) SynchronicIT B.V. 2022. All rights reserved.                           09/08/2022
#             _____                  _               ______         _             
#            / ____|                | |             |  ____|       (_)             TM
#           | (___  _   _ _ __   ___| |__  _ __ ___ | |__ _   _ ___ _  ___  _ __  
#            \___ \| | | | '_ \ / __| '_ \| '__/ _ \|  __| | | / __| |/ _ \| '_ \ 
#            ____) | |_| | | | | (__| | | | | | (_) | |  | |_| \__ \ | (_) | | | |
#           |_____/ \__, |_| |_|\___|_| |_|_|  \___/|_|   \__,_|___/_|\___/|_| |_|
#                    __/ |                                                        
#                   |___/                                 http://www.synchronicit.nl/ 
#
#  This software is confidential and proprietary of SynchronicIT and is subject to the terms and 
#  conditions defined in file 'LICENSE.txt', which is part of this source code package. You shall 
#  not disclose such Confidential Information and shall use it only in accordance with the terms 
#  of the license agreement.
#

import sfm10
import threading
import time
import atexit
import sys
import serial
import argparse
from serial.tools import list_ports


import kbhit
kbd = kbhit.KBHit()

import string
printable = set(string.printable)


parser = argparse.ArgumentParser(description='Sketch flash loader')
parser.add_argument("uart", help="uart port to use", type=str, default="", nargs='?')
parser.add_argument("baudrate", help="uart baud rate",type=int, default="115200", nargs='?')
parser.add_argument("outputFile", help="output to a file (optional)",type=str, default="", nargs='?')
args = parser.parse_args()

if str(args.uart) == "all":
    args.uart = ''

if str(args.uart).isnumeric():
    args.baudrate = args.uart
    args.uart = ''

arg_ports = str(args.uart).split(',')

print("args.baudrate = " + str(args.baudrate))

if len(arg_ports[0]) < 2:
    selection = sfm10.connect2all(args.baudrate)
else:
    selection: list[sfm10.SFM10] = sfm10.connect2(arg_ports, args.baudrate, True)

serList = []

print(selection)

f = 0
cmd = ''
for p in selection:
    ser = p.ser
    if ser:
        ser.color = sfm10.getTextColor(len(serList)+1)      
        ser.output = ''  
        ser.cntTotal = 0
        serList.append(ser)

if len(serList) > 0:
    print('\033[4m' + sfm10.bcolors.OKCYAN + "Connected to: ", end='')     
    for ser in serList:  
        print(ser.color + '[' + ser.id + '] ', end='')
    print(sfm10.bcolors.ENDC + '\n')       


def ClosePorts(event):
    global f
    if f != 0:
        f.close()
        f = 0

    if not event.is_set():
        event.set()

        for ser in serList:
            sfm10.disconnect(ser)
        print('\n\033[4m' + sfm10.bcolors.FAIL + "Closed: ", end='')     
        for ser in serList:  
            print(ser.color + '[' + ser.id + '] ', end='')
        print(sfm10.bcolors.ENDC)   
        sys.exit()

if len(args.outputFile) > 0:
    f = open(args.outputFile, "w")
    print("Started file " + args.outputFile)


def serial_rx_thread(event, serList):
    while not event.is_set():
        for ser in serList:

            try:
                cnt = ser.in_waiting
                if cnt > 0:
                    s = ''
                    err = ''
                    try:
                        array: list[int] = ser.read(cnt)

                        if 0x07 in array:
                            eidx = array.index(0x07)
                            type = array[eidx+1]
                            reason = array[eidx+2]
                            check = array[eidx+3]

                            s_pre = array[0:eidx]

                            s = s_pre.decode('utf8', errors='ignore')
                            print(hex(reason))

                            if type == 0x1 and check == 10: 
                                err += '!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n'
                                err += ser.id + ': '
                                err += 'UsageFault'
                                if reason == 0x1:
                                    err += '\nUNDEFINSTR: undefined instruction error has occurred'
                                elif reason == 0x2:
                                    err += '\nINVSTATE: an EPSR.T or EPSR.IT validity error has occurred'
                                elif reason == 0x4:
                                    err += '\nINVPC: an integrity check error has occurred'
                                elif reason == 0x8:
                                    err += '\nNOCP: a coprocessor disabled or not present error has occurred'
                                elif reason == 0x10:
                                    err += '\nSTKOF: a stack overflow error has occurred'
                                elif reason == 0x20:
                                    err += '\nUNALIGNED: an unaligned access error has occurred'
                                elif reason == 0x40:
                                    err += '\nDIVBYZERO: an integer division by zero error has occurred'
                                elif reason == 0x41:
                                    err += '\nHardFault'
                                elif reason == 0x42:
                                    err += '\nMemoryManagement'
                                elif reason == 0x43:
                                    err += '\nSecureFault'
                                elif reason == 0x44:
                                    err += '\nDebugMonitor'
                                elif reason == 0x45:
                                    err += '\nBusFault'
                                err += '\nThe device will be resetted'
                                err += '\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n'

                            s += array[(eidx+3):len(s)].decode()

                        else:

                            s = ''
                            prevWasNotAscii = True
                            for m in array:
                                if m >= 0x20 and m < 0x7F or (prevWasNotAscii and m == 0x0A):
                                    s += chr(m)
                                    prevWasNotAscii = True
                                else:
                                    s += "[" + "".join(format(m, '02X')) + "]"
                                    prevWasNotAscii = False

                        ser.output = ser.output + s 

                        if len(err) > 0:
                            ser.output = ser.output + err

                        # output = output + s
                    except Exception as e:
                        print(e)

                    if len(ser.output) > 0:
                        
                        s_ = ''.join(filter(lambda x: x in printable, ser.output))
                        idx = s_.rfind("\n")
                        if idx > 0:
                            idx = idx + 1
                            s = s_[:idx]
                            ser.output = ser.output[idx:]
                            
                            if f == 0 :
                                print('\r\033[K' + ser.color + s, end='')
                                print("\r" + sfm10.bcolors.OKCYAN + cmd + sfm10.bcolors.ENDC + "\033[K", end='')
                            if f != 0:
                                f.write(s)
                                ser.cntTotal = ser.cntTotal + 1
                                if ser.cntTotal % 50 == 0:
                                    print(str(ser.id)+ ": " + str(ser.cntTotal) + ' lines')


            except serial.SerialException as e:    
                print(e)        
                ClosePorts(event)
        time.sleep(0.02)
    ClosePorts(event)



stop_event = threading.Event()
thread = threading.Thread(target=serial_rx_thread, args=(stop_event,serList), daemon=True, name="Background")
thread.start()


def stop_background(stop_event, thread):
    stop_event.set()
    thread.join()

    
atexit.register(stop_background, stop_event, thread)


for p in selection:
    ser = p.ser
    sfm10.reset(ser)

serSelected = 0
ended = False

history = []
for ser in serList:
    history.append([])

nback = -1

if (len(serList) == 0):
    print("Could not connect to any port")
    ended = True

while not ended:
    ch = 0

    print("\r\033[K" + serList[serSelected].color + cmd + sfm10.bcolors.ENDC , end='')
    try:
        if kbd.kbhit():
            ch = ord(kbd.getch())
    except:
        ClosePorts(stop_event)
    
    if stop_event.is_set():
        ended = True
    else:
        if ch > 0:
            if ch == 0x03 :
                ClosePorts(stop_event)
            else:
                if ch == kbhit.WIN_DOWN: 
                    if nback > 0:
                        nback = nback - 1
                        cmd = history[serSelected][nback]
                        
                elif ch == kbhit.WIN_UP:
                    if len(history[serSelected]) > nback + 1:
                        nback = nback + 1
                        cmd = history[serSelected][nback]
                elif ch == kbhit.WIN_LEFT:
                    serSelected = serSelected - 1
                    if serSelected < 0:
                        serSelected = len(serList) - 1                
                elif ch == kbhit.WIN_RIGHT: 
                    serSelected = serSelected + 1
                    if serSelected >= len(serList):
                        serSelected = 0   
                elif ch == 13 or chr(ch) == '\n':
                    ser = serList[serSelected]
                    print("\r"  + sfm10.bcolors.ENDC + "TO " + ser.id + ": " + ser.color + cmd + sfm10.bcolors.ENDC)
                    ser.write(cmd.encode())
                    history[serSelected].insert(0, cmd)
                    nback = -1
                    cmd = ''
                else:
                    if (ch != 224) and (ch != '\r'):
                        cmd = cmd + chr(ch)

