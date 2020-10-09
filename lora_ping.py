import serial
import time

s = serial.Serial('/dev/ttyUSB0', 115200, timeout=2)
while True:
	cmd = 'AT+SEND=0,4,PING\r\n'.encode()
	print(cmd)
	s.write(cmd)
	print(s.readline()) # OK
	print(s.readline()) # RCV=PONG
