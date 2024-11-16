import random
from time import sleep

import serialpy
import requests
import serial.tools.list_ports
from serial.rfc2217 import Serial
import paho.mqtt.client as mqtt

URL_DEVICE_1 = "http://demo.thingsboard.io/api/v1/o0SzxyiH8fZWK1uQrRh2/telemetry"
URL_DEVICE_2 = "http://demo.thingsboard.io/api/v1/7QC2PthzFKhkp9AJAW2z/telemetry"
URL_DEVICE_3 = "http://demo.thingsboard.io/api/v1/wbUjViNjPWcBp2iyHGDy/telemetry"
URL_DEVICE_5 = "http://demo.thingsboard.io/api/v1/1EB9tCjP9BbUM7IYIqAe/telemetry"

#SERIAL_BAUDRATE = 9600
#SERIAL_COM = "COM1"

def main ():
    #TODO: connect to Acquisition Module
    count = 0

    #Main Loop
    while True:

        #TODO: Read from Acquisition Module
        #val = ser.read(1)

        #Simulation starts here

        # BOX 1
        val = random.randint(0,10)
        lock = random.randint (0,1)
        print("BOX_1 " + str(val) + " " + str(lock))
        strval = '{"ID": "1","temperature":%d, "lock": %d}' % (val, lock)
        r = requests.post(URL_DEVICE_1, data=strval)

        # BOX 2
        val = random.randint(0,10)
        lock = random.randint (0,1)
        print("BOX_2 " + str(val) + " " + str(lock))
        strval = '{"ID": "2","temperature":%d, "lock": %d}' % (val, lock)
        r = requests.post(URL_DEVICE_2, data=strval)

        # BOX 3
        val = random.randint(0,10)
        lock = random.randint (0,1)
        print("BOX_3 " + str(val) + " " + str(lock))
        strval = '{"ID": "3","temperature":%d, "lock": %d}' % (val, lock)
        r = requests.post(URL_DEVICE_3, data=strval)

        # BOX 5
        val = random.randint(0, 10)
        lock = random.randint (0,1)
        print("BOX_5 " + str(val) + " " + str(lock))
        strval = '{"ID": "5","temperature":%d, "lock": %d}' % (val, lock)
        r = requests.post(URL_DEVICE_5, data=strval)

        sleep(1)


#entry point
if __name__ == '__main__':
    main()




