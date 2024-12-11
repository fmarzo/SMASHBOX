import random
from time import sleep

import requests
from models.box import Box
import config
from models.client import Client
from firebase_db import FirebaseDB

INJECT = 0


def simulate_param(param):
    if param == config.LOCK_PARAM:
        val = 1
    elif param == config.TEMP_PARAM:
        val = 10
    return random.randint(0,val)

def main ():
    #TODO: connect to Acquisition Module

    firebase_db = FirebaseDB()

    box_1 = Box(1, config.URL_DEVICE_1)
    box_2 = Box(2, config.URL_DEVICE_2)
    box_3 = Box(3, config.URL_DEVICE_3)
    box_4 = Box(4, config.URL_DEVICE_4)
    box_5 = Box(5, config.URL_DEVICE_5)

    cli_1 = Client("El", "EL", 1001, "eltucuman1@gmail.com")
    cli_2 = Client("Tu", "TU", 1002, "eltucuman2@gmail.com")
    cli_3 = Client("Cu", "CU", 1003, "eltucuman3@gmail.com")
    cli_4 = Client("Ma", "MA", 1004, "eltucuman4@gmail.com")
    cli_5 = Client("No", "NO", 1005, "eltucuman5@gmail.com")

    if INJECT == 1:
        firebase_db.insert_new_customer(box_1, cli_1)
        firebase_db.insert_new_customer(box_2, cli_2)
        firebase_db.insert_new_customer(box_3, cli_3)
        firebase_db.insert_new_customer(box_4, cli_4)
        firebase_db.insert_new_customer(box_5, cli_5)

    #find test
    firebase_db.client_validity(1003,3)

    while True:
        #TODO: Read from Acquisition Module
        #val = ser.read(1)

        #Simulation starts here
        # BOX 1
        box_1.set_temp(simulate_param(config.TEMP_PARAM))
        box_1.set_lock(simulate_param(config.LOCK_PARAM))
        requests.post(box_1.get_url_dev(), box_1.get_packet_str())

        # BOX 2
        box_2.set_temp(simulate_param(config.TEMP_PARAM))
        box_2.set_lock(simulate_param(config.LOCK_PARAM))
        requests.post(box_2.get_url_dev(), box_2.get_packet_str())

        # BOX 3
        box_3.set_temp(simulate_param(config.TEMP_PARAM))
        box_3.set_lock(simulate_param(config.LOCK_PARAM))
        requests.post(box_3.get_url_dev(),box_3.get_packet_str())

        # BOX 4
        box_4.set_temp(simulate_param(config.TEMP_PARAM))
        box_4.set_lock(simulate_param(config.LOCK_PARAM))
        requests.post(box_4.get_url_dev(),box_4.get_packet_str())

        # BOX 5
        box_5.set_temp(simulate_param(config.TEMP_PARAM))
        box_5.set_lock(simulate_param(config.LOCK_PARAM))
        requests.post(box_5.get_url_dev(),box_5.get_packet_str())

        print("searching for client..")
        firebase_db.client_validity(1001, 1)
        sleep(1)

#entry point
if __name__ == '__main__':
    main()





