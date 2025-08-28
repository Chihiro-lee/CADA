
#!/usr/bin/env python3
import serial, sys, time
port = '/dev/ttyACM1'
ser = serial.Serial(port, 9600, timeout=None)
if not ser.is_open:
    ser.open()
    
def send_x():
    ser.write(b'x')  
    print("Sent 'x' to trigger xor_compute")
    
def send_r():
    ser.write(b'r')  
    print("Sent 'r' to trigger update")

def send_v():
    ser.write(b'v')  
    print("Sent 'v' to trigger update")
    
def send_s():
    ser.write(b's')  
    print("Sent 's' to trigger update")    

send_x()
#send_r()
#send_v()
#send_s()
ser.close()             # close port
