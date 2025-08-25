
#!/usr/bin/env python3
import serial, sys, time
port = '/dev/ttyACM1'
ser = serial.Serial(port, 9600, timeout=None)
if not ser.is_open:
    ser.open()
    
def send_c():
    ser.write(b'c')  # 发送字符 'c'
    print("Sent 's' to trigger count send")
    
def send_x():
    ser.write(b'x')  # 发送字符 'x'
    print("Sent 'x' to trigger xor_compute")
    
def send_r():
    ser.write(b'r')  # 发送字符 'r'
    print("Sent 'r' to trigger update")

def send_v():
    ser.write(b'v')  # 发送字符 'v'
    print("Sent 'v' to trigger update")
    
send_c()
#send_x()
#send_r()
#send_v()
ser.close()             # close port
