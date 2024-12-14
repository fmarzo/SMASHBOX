import random
from time import sleep
import serial
import requests
from models.box import Box
import config
from models.client import Client
from firebase_db import FirebaseDB

INJECT_CUSTOMERS = 0
SIMULATION = 0

def main ():
    #TODO: connect to Acquisition Module

    ser = serial.Serial(config.SERIAL_COM, config.SERIAL_BAUDRATE)

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

    if INJECT_CUSTOMERS == 1:
        firebase_db.insert_new_customer(box_1, cli_1)
        sleep(1)
        firebase_db.insert_new_customer(box_2, cli_2)
        sleep(1)
        firebase_db.insert_new_customer(box_3, cli_3)
        sleep(1)
        firebase_db.insert_new_customer(box_4, cli_4)
        sleep(1)
        firebase_db.insert_new_customer(box_5, cli_5)

    #find test
    #firebase_db.delete_customer(box_1,cli_1)
    #sleep(1)
    #firebase_db.delete_customer(box_2,cli_2)
    #sleep(1)
    #firebase_db.delete_customer(box_3,cli_3)
    #sleep(1)
    #firebase_db.delete_customer(box_4,cli_4)
    #sleep(1)
    #firebase_db.delete_customer(box_5,cli_5)
    #sleep(1)
    ##test on update
    #firebase_db.update_customer(box_5,cli_5, {'Names' : 'Tuccu'})
    #sleep(1)

    while True:
        #TODO: Read from Acquisition Module
        #val = ser.read(1)


        if SIMULATION == 1:
            #Simulation starts here
            # BOX 1
            box_1.simulate_box_param()
            requests.post(box_1.get_url_dev(), box_1.get_packet_str())

            # BOX 2
            box_2.simulate_box_param()
            requests.post(box_2.get_url_dev(), box_2.get_packet_str())

            # BOX 3
            box_3.simulate_box_param()
            requests.post(box_3.get_url_dev(), box_3.get_packet_str())

            # BOX 4
            box_4.simulate_box_param()
            requests.post(box_4.get_url_dev(), box_4.get_packet_str())

            # BOX 5
            box_5.simulate_box_param()
            requests.post(box_5.get_url_dev(), box_5.get_packet_str())

            print("searching for client..")
            firebase_db.find_client(1001, 1)
            sleep(1)
        else:
            val = ser.read(config.N_BYTES)
            box_1.set_box_param(val)
            requests.post(box_1.get_url_dev(), box_1.get_packet_str())
            print(val)

#entry point
if __name__ == '__main__':
    main()





