#!/usr/bin/env python3
import serial
import time
import random
import itertools


def send_cmd(self, cmd_string, timeit=False):
	cmd = f"{cmd_string}\r\n".encode()
	print(cmd)
	self.write(cmd)
	if timeit:
		t = time.time()
		print(self.readline(), end=' ') # Block until OK response
		print(time.time() - t)
	else:
		print(self.readline()) # Block until OK response

serial.Serial.cmd = send_cmd
s = serial.Serial('/dev/ttyUSB0', 115200, timeout=2)

# f must be greater than 711100000
# f must be less than   1024000000
f = 915000000 # 915MHz is default
s.cmd(f"AT+BAND={f}", timeit=True)

# Default: b'+PARAMETER=12,7,1,4\r\n'
sf, bw, cr, pp = 12, 7, 1, 4
# sf spreading factor (7-12)
# bw bandwidth 0-9 -> (7.8kHz - 500kHz)
# cr coding rate 1-4
# pp Programmed preamble 4-7
s.cmd(f"AT+PARAMETER={sf},{bw},{cr},{pp}")

s.cmd('AT+SEND=0,4,PING', timeit=True)

while True:
	# uncomment one of the lines below to perform a parameter sweep
	if True:
	#for f in range(711100000, 1024000000, 1000):
	#for sf in range(7, 13, 1):
	#for bw in range(0, 19, 1):
	#for cr in range(1, 5, 1):
	#for pp in range(3, 25, 1):
		s.cmd('AT+SEND=0,4,PING+Afterplus', timeit=True)
		print(s.readline()) # Check for receive

