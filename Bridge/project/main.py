import random
from time import sleep

import serialpy
import requests
import serial.tools.list_ports
from serial.rfc2217 import Serial
import paho.mqtt.client as mqtt
from Box import Box

#SERIAL_BAUDRATE = 9600
#SERIAL_COM = "COM1"

URL_DEVICE_1 = "http://demo.thingsboard.io/api/v1/o0SzxyiH8fZWK1uQrRh2/telemetry"
URL_DEVICE_2 = "http://demo.thingsboard.io/api/v1/7QC2PthzFKhkp9AJAW2z/telemetry"
URL_DEVICE_3 = "http://demo.thingsboard.io/api/v1/wbUjViNjPWcBp2iyHGDy/telemetry"
URL_DEVICE_5 = "http://demo.thingsboard.io/api/v1/1EB9tCjP9BbUM7IYIqAe/telemetry"

LOCK_PARAM = 1
TEMP_PARAM = 10

def simulate_param(param):
    if param == LOCK_PARAM:
        val = 1
    elif param == TEMP_PARAM:
        val = 10
    return random.randint(0,val)

def prepare_packet_str(box_obj):
    strval = '{"ID": "%d","temperature":%d, "lock": %d}' % (box_obj.id, box_obj.get_temp(), box_obj.get_lock())
    print(strval)
    return strval

def main ():
    #TODO: connect to Acquisition Module
    count = 0

    #Main Loop0,1))
    while True:

        #TODO: Read from Acquisition Module
        #val = ser.read(1)

        #Simulation starts here
        # BOX 1
        box_1 = Box(1, URL_DEVICE_1)
        box_1.set_temp(simulate_param(TEMP_PARAM))
        box_1.set_lock(simulate_param(LOCK_PARAM))
        requests.post(box_1.get_url_dev(), data=prepare_packet_str(box_1))

        # BOX 2
        box_2 = Box(2, URL_DEVICE_2)
        box_2.set_temp(simulate_param(TEMP_PARAM))
        box_2.set_lock(simulate_param(LOCK_PARAM))
        requests.post(box_2.get_url_dev(),data=prepare_packet_str(box_2))

        # BOX 3
        box_3 = Box(3, URL_DEVICE_3)
        box_3.set_temp(simulate_param(TEMP_PARAM))
        box_3.set_lock(simulate_param(LOCK_PARAM))
        requests.post(box_3.get_url_dev(),data=prepare_packet_str(box_3))

        # BOX 5
        box_5 = Box(5, URL_DEVICE_5)
        box_5.set_temp(simulate_param(TEMP_PARAM))
        box_5.set_lock(simulate_param(LOCK_PARAM))
        requests.post(box_5.get_url_dev(),data=prepare_packet_str(box_5))


        ## BOX 1
        #val = random.randint(0,10)
        #lock = random.randint (0,1)
        #print("BOX_1 " + str(val) + " " + str(lock))
        #strval = '{"ID": "1","temperature":%d, "lock": %d}' % (val, lock)
        #r = requests.post(URL_DEVICE_1, data=strval)
#
        ## BOX 2
        #val = random.randint(0,10)
        #lock = random.randint (0,1)
        #print("BOX_2 " + str(val) + " " + str(lock))
        #strval = '{"ID": "2","temperature":%d, "lock": %d}' % (val, lock)
        #r = requests.post(URL_DEVICE_2, data=strval)
#
        ## BOX 3
        #val = random.randint(0,10)
        #lock = random.randint (0,1)
        #print("BOX_3 " + str(val) + " " + str(lock))
        #strval = '{"ID": "3","temperature":%d, "lock": %d}' % (val, lock)
        #r = requests.post(URL_DEVICE_3, data=strval)
#
        ## BOX 5
        #val = random.randint(0, 10)
        #lock = random.randint (0,1)
        #print("BOX_5 " + str(val) + " " + str(lock))
        #strval = '{"ID": "5","temperature":%d, "lock": %d}' % (val, lock)
        #r = requests.post(URL_DEVICE_5, data=strval)

        sleep(1)


#entry point
if __name__ == '__main__':
    main()




