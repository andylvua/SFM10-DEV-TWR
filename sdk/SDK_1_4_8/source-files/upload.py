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
import argparse
import sys
import os

import threading
import time


parser = argparse.ArgumentParser(description='Sketch flash loader')
parser.add_argument("hex_file", help="hex file to flash")
parser.add_argument("uart", help="uart to use", default="", nargs='?')
parser.add_argument("speed", help="115200 = 0, 230400 = 1, 460800 = 2, 921600 = 3, 1843200 = 4", default="3", nargs='?')
args = parser.parse_args()

file = sfm10.hexFile(args.hex_file)

os.chdir(sys.path[0])

ver = sfm10.from32(file.version)
version = 'v' + str(ver[0]) + '.' +  str(ver[1]) + '.' + str(ver[2])  + '.' + str(ver[3]) 
size = str(file.byteCount) + ' bytes (' + str(round((file.byteCount*100)/file.maxByteCount,1)) + '%)'
all_ports = sfm10.listPorts()
arg_ports = str(args.uart).split(',')
selection = sfm10.connect2(arg_ports, 115200, False)

needsUpdate = []
valid = []
ignored = []

for d in selection:
    if d.serial > 0:
        if (d.match or file.type != "User Application"):
            valid.append(d)
        else:
            needsUpdate.append(d)
    else:
        ignored.append(d)
        if(d.ser):
            sfm10.disconnect(d.ser)

sfm10.PrintLogo(6, version)


def upload(file__, ser, speed):
    sfm10.flash(file__, ser, speed)


def uploadMulti(file_, devices_, disconnectAfter = False):
        
    serList = []
    threads = []
    lineNr = 1
    linePos = 0
    for d in devices_:
        ser = d.ser        
        ser.color = sfm10.getTextColor(lineNr)
        lineNr = lineNr + 1
        ser.blockIdx = 0
        ser.text = ser.name
        serList.append(ser)        
        x = threading.Thread(target=upload, args=(file_, ser, args.speed))
        x.start()
        threads.append(x)
        print("")
        linePos = linePos + 1
        # valid.append(d)

        
    print('\033[2A')
    while any(i.is_alive() for i in threads):
        print('\033[' + str(linePos) + 'A')
        linePos = 1
        for ser in serList:
            sfm10.PrintProgress(ser.blockIdx, file_.numberOfBlocks, ser.text, 1, 62, '\n', 6, ser.color)
            linePos = linePos + 1
        time.sleep(0.05)

    print('\033[' + str(len(serList)+1) + 'A')
    for ser in serList:
        sfm10.PrintProgress(ser.blockIdx, file_.numberOfBlocks, ser.text, 1, 62, '\n', 6, ser.color)
        
    for ser in serList:
        if (disconnectAfter):
            sfm10.disconnect(ser)


if len(needsUpdate) > 0:
    phy = sfm10.hexFile('../uwb-libraries/phy.hex')
    sfm10.printMessage("  Updating UWB libraries (" + phy.type + ")...", 'warning')
    uploadMulti(phy, needsUpdate, True)
    
for dev in needsUpdate:
    newDev = sfm10.connect2([dev.port], 115200, True)
    if len(newDev) > 0:
        valid.append(newDev[0])


used_ports_str = ''
unused_ports_str = ''
count = 0

reasons = {
    0: "unknown reason",
    1: "not a target",
    2: "could not open port",
    3: "could not connect to bootloader",
    4: "uwb library mismatch",
    5: "serial connection error",
}
reasonShow = [False] * 6

for port in all_ports:
    isUsed = False
    reason = 2
    if len(arg_ports[0]) > 1:
        reason = 1
        for p in arg_ports:
            if p.strip() == port.device:
                isUsed = True
                break
    else:
        for p in valid:
            if p.port == port.device:
                isUsed = True
                break
    if not isUsed:
        if len(unused_ports_str) > 0:
            unused_ports_str += ', '
        
        unused_ports_str += port.device

        for p in ignored:
            if p.port == port.device:
                reason = p.reason
                break

        unused_ports_str += '(' + str(reason) + ')'
        reasonShow[reason] = True
    else:
        if len(used_ports_str) > 0:
            used_ports_str += ', '
        used_ports_str += sfm10.getTextColor(count+1)
        count = count + 1
        used_ports_str += port.device


print(sfm10.bcolors.ENDC + '\n  ' + file.type + ': ' + sfm10.bcolors.BOLD + file.name + ' '*(35-len(file.name)) + sfm10.bcolors.GREY + size)
if (len(unused_ports_str)):
    print(sfm10.bcolors.GREY + '  ignoring: ' +  unused_ports_str)

    for idx, show in enumerate(reasonShow):
        if show:
            print('    ' + str(idx) + ': ' + reasons[idx])
    print('', end=sfm10.bcolors.ENDC)

if count == 0:
    print(sfm10.bcolors.FAIL + '    Unable to connect to a device.\n    Is the serial port claimed by another application?' + sfm10.bcolors.ENDC + '\n')



uploadMulti(file, valid, True)
