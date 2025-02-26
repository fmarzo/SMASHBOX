import os
from configparser import NoOptionError
from time import sleep

import requests
import serial

from init import Initializer
from models.box import Box
import config
from models.client import Client
from database.firebase_db import FirebaseDB
from mqtt.mqtt import MqttClient
from init import Initializer

def main():
    # SYSTEM INIT

    system = Initializer()
    system.init_system()
    ser = system.get_serials()
    n_ser = len(ser)
    free_ser = [0] * n_ser
    central_ser = system.get_central_serial()
    print("Acquisition serial list:" + str(ser))
    print("Central serial: " + str(central_ser))

    # MQTT alarm server
    mqtt = system.get_mqtt()
    mqtt.start_mqtt()

    while not mqtt.is_init():
        print("Wait for connection established")
        sleep(1)
        pass

    # firebase server
    firebase_db = system.get_firebase_db()

    # Entities
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

    if config.INJECT_CUSTOMERS == 1:
        firebase_db.insert_new_customer(box_1, cli_1)
        sleep(1)
        firebase_db.insert_new_customer(box_2, cli_2)
        sleep(1)
        firebase_db.insert_new_customer(box_3, cli_3)
        sleep(1)
        firebase_db.insert_new_customer(box_4, cli_4)
        sleep(1)
        firebase_db.insert_new_customer(box_5, cli_5)

    if config.DELETE_CUSTOMERS == 1:
        firebase_db.delete_customer(box_1, cli_1)
        sleep(1)
        firebase_db.delete_customer(box_2, cli_2)
        sleep(1)
        firebase_db.delete_customer(box_3, cli_3)
        sleep(1)
        firebase_db.delete_customer(box_4, cli_4)
        sleep(1)
        firebase_db.delete_customer(box_5, cli_5)
        sleep(1)

    if config.UPDATE_CUSTOMERS == 1:
        firebase_db.update_customer(box_5, cli_5, {'Names': 'Tuccu'})
        sleep(1)

    central_ok = 1

    while True:

        # ENDLESS LOOP
        #print("Central: " + str(system.get_central_serial()))
        if config.SIMULATION == 1:
            # Simulation starts here
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
            # NO SIMULATION, System is in GO
            #for ser in system.get_serials():

            if central_ok:
                central_ok = 0
                central_response = central_ser.read(config.N_BYTES)
                print("Central response: ")
                print(central_response)

                if chr(central_response[1]) == "1":
                    print("check")
                elif chr(central_response[1]) == "2":
                    #enroll
                    id_acq = central_response[2:5]
                    print(id_acq)

                    for port_name, data in ser.items():
                        if not data["busy"]:
                            data["id"] = id_acq
                            print("sto per inviare")
                            print(data["serial"])
                            data["serial"].write(id_acq)
                            break
                else:
                    print ("no response from central")

            print(ser.items())
            for port_name, data in ser.items():
                s = data["serial"]
                print(port_name)
                s.timeout = 1  # Imposta un timeout di 1 secondo
                val = s.read(config.N_BYTES)
                if not val:  # Se timeout, val sarà vuoto
                    continue  # Salta le operazioni successive e passa alla prossima porta, non voglio impostare i dati della cassetta siccome non ho letto
                box_1.set_box_param(val)
                requests.post(box_1.get_url_dev(), box_1.get_packet_str())
                print(val)
                sleep(1)

            print ("loop")

# entry point
if __name__ == '__main__':
    main()






