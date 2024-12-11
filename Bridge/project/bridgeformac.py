import random
from time import sleep

import serial
import requests
import serial.tools.list_ports
from serial.rfc2217 import Serial
import random

SERIAL_BAUDRATE = 9600
SERIAL_COM = "/dev/cu.usbmodem101"
N_BYTES = 8

#0 presence/absence
#1-2 temperature
#3-4 humidity
#5 infringement
#6 lock/unlock
#7 open/closed

def main ():
    # show port available and print it
    print("list of available ports: ")
    ports = serial.tools.list_ports.comports()

    for p in ports:
        print (p.name + p.description)

    ser = serial.Serial(SERIAL_COM, SERIAL_BAUDRATE)
    while True:
        val = ser.read(N_BYTES)
        print (val)

        if val[5].to_bytes() == b'1':
            ser.write(b"1")
            x = 0
        if val[6].to_bytes() == b'1' and x != 1:
            user_input = input("central: tipe yes if you want to unlock the box: ")
            if user_input == "yes":
                ser.write(b"2")
                x = 1
        sleep(1)

#entry point
if __name__ == '__main__':
    main()

