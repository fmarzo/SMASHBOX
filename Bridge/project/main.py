import asyncio
import os
from configparser import NoOptionError
from time import sleep

import requests
import serial
import threading

import init
from bot.tgbot import TgBot
from init import Initializer
from models.box import Box
import config
from models.client import Client
from database.firebase_db import FirebaseDB
from mqtt.mqtt import MqttClient
from init import Initializer
from config import CHAR_ENROLL, CHAR_CHECK, CHAR_UNLOCK, CHAR_IDLE


def main():
    # SYSTEM INIT

    system = Initializer()
    system.init_system()
    ser = system.get_serials()
    central_ser = system.get_central_serial()
    print("Acquisition serial list:" + str(ser))
    print("Central serial: " + str(central_ser))

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

    # MQTT alarm server
    mqtt = system.get_mqtt()
    mqtt.start_mqtt()

    while not mqtt.is_init():
        print("Wait for connection established")
        sleep(1)
        pass

    #TELEGRAM BOT
    tg_bot = TgBot(box_1, cli_1)
    # Start in a separate thread
    thread = threading.Thread(target=tg_bot.run_bot, daemon=True)
    thread.start()
    #Test msg to TG bot
    asyncio.run(tg_bot.send_msg(config.CHAT_ID_TG_BOT, "Welcome Bot"))

    while True:
        # ENDLESS LOOP
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

            central_response = central_ser.read(config.N_BYTES)
            #print("Central response: ")
            #print(central_response)
            # risolto il problema del time out, ora però c'è il rischio che andando troppo veloce legga da central una stringa vuota provocando errore
            if len(central_response) > 1:
                if chr(central_response[1]) == CHAR_IDLE:
                    pass
                elif chr(central_response[1]) == CHAR_CHECK:
                    print("check")
                    id_acq = central_response[2:5]
                    for port_name, data in ser.items():
                        if data["id"] == id_acq:
                            data["serial"].write(CHAR_UNLOCK)
                            break
                elif chr(central_response[1]) == CHAR_ENROLL:
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

                for port_name, data in ser.items():
                    if config.FORCE_SIMULATION == 0:
                        s = data["serial"]
                        if s.in_waiting > 0:  # Se ci sono dati disponibili
                            val = s.read(config.N_BYTES)
                            if val:
                                box_1.set_box_param(val)
                                requests.post(box_1.get_url_dev(), box_1.get_packet_str())
                                print(val)
                                sleep(1)
                    else:
                            box_1.simulate_box_param()
                            requests.post(box_1.get_url_dev(), box_1.get_packet_str())
                            sleep(1)
                            box_2.simulate_box_param()
                            requests.post(box_2.get_url_dev(), box_2.get_packet_str())
                            sleep(1)

# entry point
if __name__ == '__main__':
    main()






