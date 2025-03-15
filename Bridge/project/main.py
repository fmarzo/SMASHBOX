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
    box_1 = Box("-1", config.URL_DEVICE_1)
    box_2 = Box("-1", config.URL_DEVICE_2)

    box_list = list[box_1, box_2]

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

    if config.DELETE_CUSTOMERS == 1:
        firebase_db.delete_customer(box_1, cli_1)
        sleep(1)
        firebase_db.delete_customer(box_2, cli_2)
        sleep(1)

    if config.UPDATE_CUSTOMERS == 1:
        firebase_db.update_customer(box_2, cli_2, {'Names': 'Tuccu'})
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

            print("searching for client..")
            firebase_db.find_client(1001, 1)
            sleep(1)
        else:
            # NO SIMULATION, System is in GO

            central_response = central_ser.read(config.N_BYTES)
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
                    print(f"id_acq: {id_acq}")
                    for port_name, data in ser.items():
                        if not data["busy"]:
                            data["id"] = id_acq
                            print("sto per inviare")
                            print(data["serial"])
                            data["serial"].write(id_acq)
                            data["busy"] = True
                            #update box_id according with what received
                            for b in box_list:
                                if b.get_id() == "-1":
                                    b.set_id(id_acq)
                                    break
                            break

                for port_name, data in ser.items():
                        s = data["serial"]
                        if s.in_waiting > 0:  # Se ci sono dati disponibili
                            val = s.read(config.N_BYTES)
                            if val:
                                id_comm = val[2:5]
                                for b in box_list:
                                    if b.id == id_comm:
                                        b.set_box_param(val)
                                        requests.post(box_1.get_url_dev(), box_1.get_packet_str())
                                        print(val)
                                        break
                            print(val)
                            sleep(1)

# entry point
if __name__ == '__main__':
    main()






