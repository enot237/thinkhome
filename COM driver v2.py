#!/usr/bin/env python3
import serial
import time
import socket
WEBPORT = 9090 #port to listen
sock = socket.socket() #up socket
for i in range(9090,9100): #try using ports in range 9090-9100
    try:
        sock.bind(('', i))
        print("successfull binded for {0} port".format(i))
        WEBPORT = i
        break #if port successfully binded, break cycle "for"
    except:
        print("ca't use {0} port, checking next...".format(i))
sock.listen(1) #when port binded, listening
range_stop = 10
for i in range(0,range_stop+1): #this cycle trying to bind COM port to receive data to atmega
    try:
        ser = serial.Serial("/dev/ttyACM{0}".format(i), 9600)
        print("successfull binded for {0} COM port".format(i))
        break
    except:
        print("can't use {0} COM port, checking next...".format(i))
        if i==range_stop:
            print("can't bind to COM port, it may be unpluged \nPress Enter to stop programm")
            input()
            exit()
print("tuning /home/anisimov/nginx/web/php/socket.php")
script = open('/home/anisimov/nginx/web/php/socket.php', 'r+') #open php script, which we need to tuning, r+ means read this
a = ""
for line in script:
    if '$service_port = ' in line:
        line = '$service_port = '+ str(WEBPORT)+"; \n"
        print("changing port")
    a+=line
script.close()
new_script = open('/home/anisimov/nginx/web/php/socket.php', 'w') # 'w' means rewrite this
new_script.write(a)
new_script.close()

print("start checking system...")
for i in range(0,5): #i use that cycle to check working
    ser.write(b'0')
    time.sleep(0.1)
    c = ser.read_all()
print("system setup completed!")




print("Start working")
while True:
    conn, addr = sock.accept() #listen socket, get data and recieve that data to COM port
    print('connected:', addr)
    data = conn.recv(32)
    print("client send data:" , data) #print data, which will be recieved


    ser.write(data)
    time.sleep(0.1)
    c = ser.readline()


    print("arduino send data:", c) #print data,which we get from com port
    conn.sendall(c)
    conn.close()


