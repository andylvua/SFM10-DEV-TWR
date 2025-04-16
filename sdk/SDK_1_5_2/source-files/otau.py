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
import subprocess
import math

parser = argparse.ArgumentParser(description='Sketch flash loader')
parser.add_argument("hex_file", help="hex file to flash")
parser.add_argument("uart", help="uart to use", default="", nargs='?')
parser.add_argument("speed", help="115200 = 0, 230400 = 1, 460800 = 2, 921600 = 3, 1843200 = 4", default="3", nargs='?')
args = parser.parse_args()


def calcIntelHexLineCRC(bytes):
    return 256 - sum(bytes) & 0xff


file = sfm10.hexFile(args.hex_file)

fin = open(args.hex_file, "rt")
new_hex_file = args.hex_file + '-otau.hex'
fout = open(new_hex_file, "wt")


pageCount = math.ceil((file.byteCount) / 512)
print("pageCount = " + str(pageCount))

lineNr = 0
for line in fin:
    lineNr = lineNr + 1
    if lineNr == 2:

        l2 = line[:14] + '10F' + line[17:41]
        numbers = bytearray.fromhex(l2[1:len(l2)])
        l2 += format(calcIntelHexLineCRC(numbers), '02X')
        l2 += '\n'
        fout.write(l2)
    elif lineNr == 3:
        
        l2 = line[:23]
        l2 += format(pageCount, '02X')
        l2 += line[25:41]

        numbers = bytearray.fromhex(l2[1:len(l2)])
        l2 += format(calcIntelHexLineCRC(numbers), '02X')

        l2 += '\n'
        fout.write(l2)
    else:
        
        fout.write(line)
#close input and output files
fin.close()
fout.close()


sys.argv[0]  = sys.argv[0].replace('otau', 'upload')
newargs = " ".join(x for x in sys.argv)
newargs = newargs.replace(args.hex_file, new_hex_file)

print(sys.argv)
print(newargs)
subprocess.Popen("python " + newargs,shell=True)