import random
from time import sleep

import serialpy
import requests
import configurations
import serial.tools.list_ports
import Box
from serial.rfc2217 import Serial

URL_DEVICE_1 = "http://demo.thingsboard.io/api/v1/o0SzxyiH8fZWK1uQrRh2/telemetry"
URL_DEVICE_2 = "http://demo.thingsboard.io/api/v1/7QC2PthzFKhkp9AJAW2z/telemetry"
URL_DEVICE_3 = "http://demo.thingsboard.io/api/v1/wbUjViNjPWcBp2iyHGDy/telemetry"

#SERIAL_BAUDRATE = 9600
#SERIAL_COM = "COM1"

def main ():
    # show port available and print it
    print("list of available ports: ")
    ports = serial.tools.list_ports.comports()

   #for p in ports:
   #    print(("Arduino Uno " + "(" + p.name + ")"))
   #    if (p.description == ("Arduino Uno " + "(" + p.name + ")")):
   #        ser = serial.Serial(p.name, SERIAL_BAUDRATE)
   #        break

    while True:
        #val = ser.read(1)

        # BOX 1
        val = random.randint(0,10)
        lock = 0
        #lock = random.randint (0,1)
        print("BOX_1 " + str(val) + " " + str(lock))
        strval =  "{temperature:%d, lock:%d}" % (val,lock)
        r = requests.post(URL_DEVICE_1, data=strval)

        # BOX 2
        val = random.randint(0,10)
        #lock = random.randint (0,1)
        lock = 0
        print("BOX_2 " + str(val) + " " + str(lock))
        strval =  "{temperature:%d, lock:%d}" % (val,lock)
        r = requests.post(URL_DEVICE_2, data=strval)

        # BOX 3
        val = random.randint(0,10)
        #lock = random.randint (0,1)
        lock = 0
        print("BOX_3 " + str(val) + " " + str(lock))
        strval =  "{temperature:%d, lock:%d}" % (val,lock)
        r = requests.post(URL_DEVICE_3, data=strval)

        sleep(1)

#entry point
if __name__ == '__main__':
    main()


