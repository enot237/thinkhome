#!/usr/bin/env python3
import serial
import time
import socket
WEBPORT = 9090 #socket port variable
sock = socket.socket()
for i in range(9090,9100): #try to use ports in range 9090-9100
    try:
        sock.bind(('', i))
        print("successfull binded for {0} port".format(i))
        WEBPORT = i
        break
    except:
        print("ca't use {0} port, checking next...".format(i))
sock.listen(1)
range_stop = 10 #number of comport to using
for i in range(0,range_stop+1):
    try:
        ser = serial.Serial("/dev/ttyACM{0}".format(i), 9600)
        print("successfull binded for {0} COM port".format(i))
        break
    except:
        print("can't use {0} COM port, checking next...".format(i))
        if i==range_stop:
            print("can't bind to COM port, it may be unpluged")
            exit()
print("tuning socket.php")
#start tuning php script
script = open('socket.php', 'r+')
a = ""
for line in script:
    if '$service_port = ' in line:
        line = '$service_port = '+ str(WEBPORT)+"; \n"
        print("changing port")
    a+=line
script.close()
new_script = open('socket.php', 'w')
new_script.write(a)
new_script.close()
#end tuning php script
#start checking system, write bytes '0' to selected com port to check working
print("start checking system...")
for i in range(0,5):
    ser.write(b'0')
    time.sleep(0.1)
    c = ser.read_all()
print("system setup completed!")




print("Start working")
while True:
    conn, addr = sock.accept()
    print('connected:', addr)
    data = conn.recv(32)
    print("client send data:" , data)


    ser.write(data)
    time.sleep(0.1)
    c = ser.readline()


    print("arduino send data:", c)
    conn.sendall(c)
    conn.close()

