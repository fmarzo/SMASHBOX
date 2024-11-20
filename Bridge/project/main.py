import random
from time import sleep

import serialpy
import requests
import serial.tools.list_ports
from serial.rfc2217 import Serial
import paho.mqtt.client as mqtt
from Box import Box
import config

def simulate_param(param):
    if param == config.LOCK_PARAM:
        val = 1
    elif param == config.TEMP_PARAM:
        val = 10
    return random.randint(0,val)

def prepare_packet_str(box_obj):
    strval = '{"ID": "%d","temperature":%d, "lock": %d}' % (box_obj.id, box_obj.get_temp(), box_obj.get_lock())
    print(strval)
    return strval

def main ():
    #TODO: connect to Acquisition Module

    while True:

        #TODO: Read from Acquisition Module
        #val = ser.read(1)

        #Simulation starts here
        # BOX 1
        box_1 = Box(1, config.URL_DEVICE_1)
        box_1.set_temp(simulate_param(config.TEMP_PARAM))
        box_1.set_lock(simulate_param(config.LOCK_PARAM))
        requests.post(box_1.get_url_dev(), data=prepare_packet_str(box_1))

        # BOX 2
        box_2 = Box(2, config.URL_DEVICE_2)
        box_2.set_temp(simulate_param(config.TEMP_PARAM))
        box_2.set_lock(simulate_param(config.LOCK_PARAM))
        requests.post(box_2.get_url_dev(),data=prepare_packet_str(box_2))

        # BOX 3
        box_3 = Box(3, config.URL_DEVICE_3)
        box_3.set_temp(simulate_param(config.TEMP_PARAM))
        box_3.set_lock(simulate_param(config.LOCK_PARAM))
        requests.post(box_3.get_url_dev(),data=prepare_packet_str(box_3))

        # BOX 4
        box_4 = Box(4, config.URL_DEVICE_4)
        box_4.set_temp(simulate_param(config.TEMP_PARAM))
        box_4.set_lock(simulate_param(config.LOCK_PARAM))
        requests.post(box_4.get_url_dev(),data=prepare_packet_str(box_3))

        # BOX 5
        box_5 = Box(5, config.URL_DEVICE_5)
        box_5.set_temp(simulate_param(config.TEMP_PARAM))
        box_5.set_lock(simulate_param(config.LOCK_PARAM))
        requests.post(box_5.get_url_dev(),data=prepare_packet_str(box_5))

        sleep(1)

#entry point
if __name__ == '__main__':
    main()




