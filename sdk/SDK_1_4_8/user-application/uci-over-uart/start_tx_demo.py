#!/usr/bin/env python

import sys
import serial
from serial.tools import list_ports

import time
import phy


if(len(sys.argv) != 2):
	sys.stderr.write('No argument given\nAvailable ports:\n')
	ports = list_ports.comports()
	for port in ports:
		sys.stderr.write('{}\t: {}\n'.format(port.name, port.device))
		sys.exit(1)

# connect to serial port
ser = serial.serial_for_url(sys.argv[1], do_not_open=True)
ser.baudrate = 115200
ser.bytesize = 8
ser.parity = 'N'
ser.stopbits = 1
ser.rtscts = False
ser.xonxoff = False


try:
	ser.open()
except serial.SerialException as e:
	sys.stderr.write('Could not open serial port {}: {}\n'.format(ser.name, e))
	sys.exit(1)

print("{} is opened.\n".format(sys.argv[1]))

def ser_write(d, data):
	s = str(bytearray(data)) if sys.version_info<(3,) else bytes(data)
	return d.write(s)

def ser_read(d, nbytes):
	s = d.read(nbytes)
	return [ord(c) for c in s] if type(s) is str else list(s)

# reset device:
ser.reset_input_buffer()
time.sleep(0.01)
ser.dtr = True  
time.sleep(0.01)
ser.dtr = False

time.sleep(0.25)


def print_uci(bytes):
	if len(bytes) < 5:
		return
	l = bytes[3]
	s = bytes[0:(4+l)]
	remainder = bytes[(4+l):]
	res = ''
	if s[0] in phy.byte0:
		res += phy.byte0[s[0]]
		res += ': '
		if s[1] in phy.byte1[s[0]]:
			res += phy.byte1[s[0]][s[1]]
	res += ' (' + phy.byte4[s[4]] + ')'
	if s[3] > 1:
		res += ': ' + ' '.join('{:02x}'.format(x) for x in s[5:])
	print(res)
	if (len(remainder) > 0):
		print_uci(remainder)



print_uci(ser_read(ser, ser.in_waiting))




def send_uci(message):
	ser.reset_input_buffer()
	ser_write(ser, message)
	time.sleep(0.2)
	# READ BACK RESPONSE
	cnt = ser.in_waiting
	return ser_read(ser, cnt)


def send_cmd(byte0, byte1, payload):
	s = send_uci([byte0, byte1, 0x00, len(payload)] + payload)
	print_uci(s)



def uwb_begin():
	send_cmd(phy.ACTIONS_CMD, phy.ACTIONS_BEGIN, [])


def uwb_set_payload(payload):
	send_cmd(phy.CONFIGURATION_CMD, phy.CONFIGURATION_SET_PAYLOAD, [len(payload), 0x00] + payload)


def uwb_start_tx():
	send_cmd(phy.ACTIONS_CMD, phy.ACTIONS_START_TX, [0, 0, 0, 0, 0, 0, 0, 0])



uwb_begin()
uwb_set_payload([0x61, 0x61, 0x70])
uwb_start_tx()


