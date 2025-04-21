import random
from time import sleep
import struct
import serial
import requests
import serial.tools.list_ports
from serial.rfc2217 import Serial
import random

SERIAL_BAUDRATE = 9600
SERIAL_COM = "COM7"
N_BYTES = 7

#0 ID
#1 presence/absence
#2-3 temperature
#4-5 humidity
#6 infringement
#7 lock/unlock
#8 open/closed

def main ():
    # show port available and print it
    print("list of available ports: ")
    ports = serial.tools.list_ports.comports()

    for p in ports:
        print (p.name + p.description)

    ser = serial.Serial(SERIAL_COM, SERIAL_BAUDRATE)
    while True:
        raw = ser.read(N_BYTES)
        id = struct.unpack('<7B', raw)

        id_, pres, temp, humidity, infr, lock, open_ = struct.unpack('7B', raw)
        print(
            f"ID: {id_}, Presence: {pres}, Temp: {temp}, Humidity: {humidity:}, Infra: {infr}, Lock: {lock}, Open: {open_}")


#entry point
if __name__ == '__main__':
    main()
