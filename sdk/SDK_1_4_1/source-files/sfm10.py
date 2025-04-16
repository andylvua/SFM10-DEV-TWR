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


import serial
import time
import sys
import version
from typing import NamedTuple
from serial.tools import list_ports
import math
from intelhex import IntelHex
import six
import os
import itertools
import zlib
import threading

def to32(v):
    if (len(v) == 4):
        return (v[0] << 24) + (v[1] << 16) + (v[2] << 8) + v[3]
    else:
        return 0

def from32(v: int):
    # return [v & 0xFF, (v>>8) & 0xFF, (v>>16) & 0xFF, (v>>24) & 0xFF]
    return [ (v>>24) & 0xFF,  (v>>16) & 0xFF, (v>>8) & 0xFF, v & 0xFF]

class SFM10(NamedTuple):
    hardware: int
    application: int
    bootloader: int
    serial: int
    type: int
    status: int
    version: int
    port: str
    match: bool
    ser: any
    reason: int

class HEXFILE(NamedTuple):
    name:str
    version: int
    type: str
    blockSize: int
    numberOfBlocks: int
    byteCount: int
    maxByteCount: int
    data: any
    address: list


LINE_LENGTH = 60

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    GREY = '\033[90m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def getTextColor(i):
    return '\033[38;5;' + str(i) + 'm'

def type2str(type):
    if type == 0x01:
        return "UCI Ranging App"
    if type == 0x02:
        return "PHY UWB Library"
    if type == 0x20:
        return "SFS Production"
    if type == 0x21:
        return "Bootloader Update"
    if type == 0xA0:
        return "User Application"
    if type == 0xA1:
        return "Application OTAU"
    return "Unknown"
    
def print_and_exit(error_text):
    print('\n' + bcolors.FAIL + error_text + bcolors.ENDC)
    sys.exit()

def sdkVersion():
    return to32([version.MAJOR, version.MINOR, version.PATCH, version.DATE])

def reset(ser):
    if ser:
        ser.dtr = True  
        time.sleep(0.01)
        ser.dtr = False

def disconnect(ser): 
    ser.rts = False  
    ser.close()
    ser.is_open = False

def connect(port, baudrate = 0):
    port = str(port).strip()
    if baudrate == 0:
        baudrate = 115200
    try:
        ser = serial.serial_for_url(port, do_not_open=True)
        ser.baudrate = baudrate
        ser.bytesize = 8
        ser.parity = 'N'
        ser.stopbits = 1
        ser.rtscts = False
        ser.xonxoff = False
        ser.id = port        
        ser.open()
    except serial.SerialException as e:
        return 0
        # print_and_exit("Could not open serial port {}: {}\nIs it connected? Does some other application claim the port?".format(ser.name, e))
    
    ser.reset_input_buffer()
    reset(ser)
    ser.rts = True  
    ser.is_open = True
    return ser

def write(d, data):
    s = str(bytearray(data)) if sys.version_info<(3,) else bytes(data)
    return d.write(s)

def read(d, nbytes):
    s = d.read(nbytes)
    return [ord(c) for c in s] if type(s) is str else list(s)


def PrintProgress (iteration, total, text = '', decimals = 1, length = 62, printEnd = "\r", color=6, textColor=bcolors.ENDC):
    percent = ("{0:." + str(decimals) + "f}").format(100 * (iteration / float(total)))
    filledLength = int(length * iteration // total)
    filled = text + ' ' * (length - len(text))
    bar = '\x1b[1;4' + str(color) + ';4' + str(color) + 'm' + filled[0:filledLength] + bcolors.ENDC + filled[filledLength:length]
    print(f'\r {textColor} {bar} {textColor} {percent}%', end = printEnd)



def PrintLogo(color, text):
    rows = [
        "       _____                  __                     _      __________",
        "      / ___/__  ______  _____/ /_  _________  ____  (_)____/  _/_  __/",
        "      \__ \/ / / / __ \/ ___/ __ \/ ___/ __ \/ __ \/ / ___// /  / /   ",
        "     ___/ / /_/ / / / / /__/ / / / /  / /_/ / / / / / /___/ /  / /    ",
        "    /____/\__, /_/ /_/\___/_/ /_/_/   \____/_/ /_/_/\___/___/ /_/     ",
        "         /____/  " + text,
    ]
    for i in range(0, len(rows)):
        print(getTextColor(color) + rows[i] + bcolors.ENDC)


        
def PrintInfo(info: SFM10):
    bl = from32(info.bootloader)
    bootloader = str(bl[2]) + '.' +  str(bl[1]) + '.' + str(bl[0]) + '-' + str(bl[3])

    
    ver = from32(info.version)
    ver = 'v' + str(ver[0]) + '.' +  str(ver[1]) + '.' + str(ver[2])  + '.' + str(ver[3]) 
    # ver_year = math.floor(ver[3] / 0x10) + 2020
    # ver_month = ver[3] % 0x10
    # version += '(' + str(ver_month) + '/' + str(ver_year) + ')'


    if info.serial > 0 and not info.match:
        ver = bcolors.FAIL + ver + bcolors.ENDC

    bootloader_date = bl[2] * 10000 + bl[1] * 100 + bl[3]
    oldLoader = info.serial > 0 and bootloader_date < version.BOOTLOADER
    if oldLoader:
       bootloader = bcolors.FAIL + bootloader + bcolors.ENDC

    if info.type > 0 or info.serial > 0:
        moreInfo = ' | bl=' + bootloader + ' hw=0x' + str(format(info.hardware >> 16, 'X') + "/" + format(info.hardware & 0xFFFF, 'X')) + ' app=0x' + str(format(info.application, 'X')) + ' ' +  'ser=0x' + str(format(info.serial, '08X'))
    else:
        moreInfo = ''
    print(bcolors.OKCYAN + type2str(info.type) + bcolors.ENDC + ' ' + ver + ' @ ' + info.port + moreInfo)

def printMessage(text: str, type: str):
    if type == 'warning':
        text = bcolors.WARNING + text + bcolors.ENDC
    if type == 'error':
        text = bcolors.FAIL + text + bcolors.ENDC
    print(text)


def info(ser):
    if not ser:
        return SFM10(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2)
    
    baudrate = ser.baudrate
    if baudrate != 115200:
        ser.baudrate = 115200
    reset(ser)
    time.sleep(0.05)
    s = read(ser, ser.in_waiting)
    instruction = 5
    addr = 0x201000
    write(ser, [instruction, 0x3F, 7, 0xFF, 0xFF, addr & 0xFF, (addr>>8) & 0xFF, (addr>>16) & 0xFF, (addr>>24) & 0xFF ])
    time.sleep(0.05)
    cnt = ser.in_waiting
    s = read(ser, cnt)
    if cnt == 0:
        info = SFM10(0, 0, 0, 0, 0, 1, 0, ser.name, 0, 0, 3)
    else:
        version = to32(s[20:24])
        match = sdkVersion() == version
        if (match == False):
            print("  -> found: " + hex(version) + " but should be version " + hex(sdkVersion()))
        application = s[2:6]
        application.reverse()
        hardware = s[6:10]
        hardware.reverse()
        info = SFM10(to32(hardware), to32(application), to32(s[10:14]), to32(s[14:18]), s[18], s[19], version, ser.name, match, ser, 0)
    
    
    if baudrate != 115200:
        ser.baudrate = baudrate

    return info


def listPorts():
    return list_ports.comports()


def connectWorker(portStr, baudrate, SFM10_list):
    ser = connect(portStr, baudrate)
    if (ser):
        item = info(ser)
        SFM10_list.append(item)
    else:
        SFM10_list.append(SFM10(0, 0, 0, 0, 0, 0, 0, 0, portStr, 0, 5))


def connect2ports(portNames, baudrate):
    threads = []
    results = []
    for portStr in portNames: 
        x = threading.Thread(target=connectWorker, args=(portStr, baudrate, results))
        x.start()
        threads.append(x)
    
    portNames.sort()
    while any(i.is_alive() for i in threads):
        time.sleep(0.001)


    sortedResults = []
    for portStr in portNames: 
        for item in results:
            if item.port == portStr:
                sortedResults.append(item)

    return sortedResults


def connect2all(baudrate):
    ports = listPorts()
    portNames = []
    for port in ports:
        portNames.append(port.device)
    return connect2ports(portNames, baudrate)



def connect2(arg_ports, baudrate, validOnly: bool):
    selection = []
    if len(arg_ports[0]) < 2:
        selection = connect2all(baudrate)
    else:
        selection = connect2ports(arg_ports, baudrate)
        for p in selection:
            if not p.ser:
                printMessage("Could not open " + p.port + ". Is it connected? Does some other application claim the COM port?", 'warning')

    if validOnly:
        valid = []
        for d in selection:
            if d.serial > 0 and d.match:
                valid.append(d)
        return valid
    return selection


def hexFile(hex_file: str):
        
    # Check for valid path first.
    isPath = isinstance(hex_file, six.string_types)
    if isPath and not os.path.isfile(hex_file):
        print_and_exit("No such file: '{}'".format(hex_file))


    # If no format provided, use the file's extension.

    if isPath:
        # Extract the extension from the path.
        file_format = os.path.splitext(hex_file)[1][1:]
        
        # Explicitly check for no extension.
        if file_format == '':
            print_and_exit("file path '{}' does not have an extension and "
                                "no format is set".format(hex_file))
    else:
        print_and_exit("file object provided but no format is set")

    # Check the format is one we understand.
    if file_format != 'hex':
        print_and_exit("unknown file format '%s'" % file_format)


    def ranges(i):
        """!
        Accepts a sorted list of byte addresses. Breaks the addresses into contiguous ranges.
        Yields 2-tuples of the start and end address for each contiguous range.
        
        For instance, the input [0, 1, 2, 3, 32, 33, 34, 35] will yield the following 2-tuples:
        (0, 3) and (32, 35).
        """
        for a, b in itertools.groupby(enumerate(i), lambda x: x[1] - x[0]):
            b = list(b)
            yield b[0][1], b[-1][1]

    def readData(fileName):

        file_obj = None
        try:
            # Open the file if a path was provided.
            if isPath:
                mode = 'rb'
                if file_format == 'hex':
                    mode = 'r' # hex file must be read as plain text file
                file_obj = open(fileName, mode)
            else:
                file_obj = fileName

            hexfile = IntelHex(file_obj)
            addresses = hexfile.addresses()

            prevAddr = addresses[0]
            newAddresses = []
            newData = []
            for i in range(len(addresses)):
                a = addresses[i]
            # for a in addresses:
                filler = prevAddr + 1

                while filler < a:
                    newAddresses.append(filler)
                    newData.append(0)
                    filler += 1
                prevAddr = a
                newAddresses.append(a)
                newData.append(0)

            addresses = newAddresses
            data = newData
            data_list = list(ranges(addresses))
            if (len(data_list) != 1):
                print_and_exit("Reading file error: Incorrect number of sections")
            for start, end in data_list:
                size = end - start + 1
                data = list(hexfile.tobinarray(start=start, size=size))
                return data, start
        finally:
            if isPath and file_obj is not None:
                file_obj.close()
                
    data, address = readData(hex_file)

    blockSize = 7
    bs = 2**blockSize
    numberOfBlocks = math.ceil(len(data) / bs)
    addPadding = numberOfBlocks * bs - len(data)
    listofzeros = [0] * addPadding
    data = data + listofzeros
    
    version = to32(data[4:8])
    absPath = os.path.abspath(hex_file)
    names = (str(absPath).replace('\\', '/').split('/'))
    projectName = ''
    for name in reversed(names):
        if name != 'app.hex' and name != 'build':
            projectName = name
            break
        
    byteCount = len(data)
    if data[2] == 0xA0:
        maxByteCount = 0x0021FFFF - 0x00210000
    else:
        maxByteCount = 0xEC00


    return HEXFILE(projectName, version, type2str(data[2]), blockSize, numberOfBlocks, byteCount, maxByteCount, data, address)


def flash(file: HEXFILE, ser, speed: int = 3):
    reset(ser)
    time.sleep(0.05)
    s = read(ser, ser.in_waiting)

    # SEND FIRMWARE UPDATE COMMAND

    instruction = int(speed) + 5
    numb = file.numberOfBlocks
    addr = from32(file.address)
    write(ser, [instruction, 0x3F, file.blockSize, numb & 0xFF, (numb>>8) & 0xFF, addr[3], addr[2], addr[1], addr[0]])


    time.sleep(0.05)

    # READ BACK RESPONSE

    cnt = ser.in_waiting
    s = read(ser, cnt)



    if cnt == 0:
        print_and_exit("The device does not answer back. Make sure the RX, TX, and RESET lines are connected and the device is powered.")
        
    else:
        
        response_code = s[0]
        
        if response_code == 0x61: # 'a'
            print_and_exit("The target memory address is out of range.")
        if response_code == 0x73: # 's':
            print_and_exit("The application does not fit in flash.")

        if cnt < 24:
            print("\nReceived:")
            print(' '.join('{:02x}'.format(x) for x in s))
            print_and_exit("The device did not answer back in an expected manner. Try uploading with the lowest speed: e.g. build/app.hex COM15 0")


    time.sleep(0.01)

    if (int(speed) > 0):
        ser.baudrate = 115200 << int(speed)


    # INTERPRET RESPONSE

    hardware_id = s[2:6]
    hardware_id.reverse()

    application_id = s[6:10]
    application_id.reverse()

    bootloader_id = s[10:14]
    serial_number = s[14:18]
    # serial_number.reverse()

    bootloader_date = bootloader_id[2] * 10000 + bootloader_id[1] * 100 + bootloader_id[3]



    hardware_id_str = "0x" + "".join(format(x, '02X') for x in hardware_id)
    application_id_str = "0x" + "".join(format(x, '02X') for x in application_id)
    serial_number_str = "0x" + "".join(format(x, '02X') for x in serial_number)
    bootloader_id_str = str(bootloader_id[2]) + '.' +  str(bootloader_id[1]) + '.' + str(bootloader_id[0]) + ' v' + str(bootloader_id[3])

    # START TRANSFER


    MAX_RETRIES = 20
    blockIdx = 0
    blockRetries = 0
    # print("\n")

    bs = 2**file.blockSize

    while blockIdx < numb and blockRetries < MAX_RETRIES:


        header = [0x01, 0x3F, (blockIdx & 0xFF), (blockIdx >> 8) & 0xFF]

        iStart = blockIdx * bs
        iEnd = iStart + bs

        write(ser, header + file.data[iStart:iEnd])
        t = zlib.crc32(bytes(header + file.data[iStart:iEnd]))

        start = time.time()
        now = time.time()
        cnt = ser.in_waiting
        while cnt < 4 and ((now - start) < 2.0):
            cnt = ser.in_waiting
            now = time.time()

        cnt = ser.in_waiting
        s = read(ser, cnt)


        if cnt != 4:

            response_code = 0
            if cnt > 0:
                response_code = s[0]
            if response_code == 0x68: # 'h'
                print_and_exit("The device does not accept this HEX file: incorrect header structure.")
            else:
                print("\nReceived:")
                print(' '.join('{:02x}'.format(x) for x in s))
                print_and_exit("An unexpected number of bytes are received.")

        crcIn = int.from_bytes( bytes(s[0:4]), "little")

        if crcIn == t:
            # print("", end=".", flush=True)
            blockIdx += 1
            # printProgressBar(blockIdx, numberOfBlocks, port)
            text = " " + ser.name + ' '*(LINE_LENGTH-len(ser.name)-10) + serial_number_str
            ser.text = text
            ser.blockIdx = blockIdx
            # PrintProgress(blockIdx, numb, text)
        else:
            blockRetries += 1

                
        if (blockIdx % 6 == 5):
            ser.rts = True
        if (blockIdx % 6 == 0):
            ser.rts = False
            

    if blockRetries >= MAX_RETRIES:
        print_and_exit("Writing to flash failed, because too many incorrect CRCs are received. Check the RX/TX connections.")

    # finalize
    empty_block = [0] * (bs - 4)
    # totalCRC2 = zlib.crc32(bytes(data[32:len(data)]))
    
    if bootloader_date < 220804:
        totalCRC = zlib.crc32(bytes(file.data))
    else:
        totalCRC = zlib.crc32(bytes(file.data[32:len(file.data)]))


    write(ser, [0x04, 0x3F, 0, 0] + [totalCRC & 0xFF, (totalCRC>>8) & 0xFF, (totalCRC>>16) & 0xFF, (totalCRC>>24) & 0xFF] + empty_block)
    time.sleep(0.05)
    cnt = ser.in_waiting
    s = read(ser, cnt)

    crcIn = int.from_bytes( bytes(s[0:4]), "little")

    if crcIn != totalCRC:
        print_and_exit("The total checksum is incorrect. This could be due to corrupt flash.")


    ser.rts = False
