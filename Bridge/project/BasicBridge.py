import random
from time import sleep

import serialpy
import requests
import serial.tools.list_ports
from serial.rfc2217 import Serial

SERIAL_BAUDRATE = 9600
SERIAL_COM = "COM1"
N_BYTES = 1

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
        sleep(1)

#entry point
if __name__ == '__main__':
    main()


