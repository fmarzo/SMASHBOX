import asyncio
import json
import os
import struct
import time
from configparser import NoOptionError
from random import randint
from time import sleep

import requests
import serial
import threading

from pycparser.ply.lex import TOKEN

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
from project.config import UBI_TOKEN, UBI_BROKER, UBI_PORT, TB_TOKEN, BROKER, TB_HOST, TB_PORT, TB_TOPIC, CODE_UNLOCK, \
    CODE_ENROLL


def read_central(central_ser, ser, box_list):
    while True:
        central_response = central_ser.read(config.CNTR_N_BYTES)
        if len(central_response) > 1:
            print(central_response)
            header, id = struct.unpack('<2B', central_response)
            print(f"header {header}, fingerID {id}")

            if id == CHAR_IDLE:
                pass
            elif id == CODE_UNLOCK:
                print("check")
                for port_name, data in ser.items():
                    if data["id"] == id:
                        data["serial"].write(CHAR_UNLOCK)
                        break
            elif id == CODE_ENROLL:
                print("enroll")
                for port_name, data in ser.items():
                    if not data["busy"]:
                        data["id"] = id
                        print("sto per inviare")
                        print(data["serial"])
                        data["serial"].write(id)
                        data["busy"] = True
                        # update box_id according with what received
                        for b in box_list:
                            if b.get_id() == "-1":
                                b.set_id(id)
                                break
                        break

def read_serial_loop(s, box_list, client):
    print("In thread")
    while True:
        if s.in_waiting >= config.N_BYTES:
            val = s.read(config.N_BYTES)
            id_, pres, temp, humidity, infr, lock, open_ = struct.unpack('7B', val)
            for b in box_list:
                if b.get_id() == id_:
                    b.set_raw_box_param(val)
                    payload = b.get_packet()
                    client.publish(config.TB_TOPIC, json.dumps(payload))
                    print(
                        f"ID: {id_}, Presence: {pres}, Temp: {temp}, Humidity: {humidity:}, Infra: {infr}, Lock: {lock}, Open: {open_}")
                    #requests.post(b.get_url_dev(), b.get_packet_str())
                    time.sleep(1)
                    break

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
    box_1 = Box(b"122", config.URL_DEVICE_1)
    box_2 = Box("-1", config.URL_DEVICE_2)

    box_list = [box_1, box_2]

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

    import paho.mqtt.client as mqtt
    import json

    # Token dispositivo (usato come username)

    # 1. Crea il client
    client = mqtt.Client()

    # 2. Imposta username (token)
    client.username_pw_set(TB_TOKEN)

    # 3. Connetti al broker
    client.connect(TB_HOST, TB_PORT, 60)

    # 5. Disconnetti (dopo breve pausa se necessario)
    #client.disconnect()

    #client.loop_stop()
    #client.disconnect()

    #mqtt = system.get_mqtt()
    #mqtt.start_mqtt()

    #while not mqtt.is_init():
    #    print("Wait for connection established")
    #    sleep(1)
    #    pass

    #TELEGRAM BOT
    tg_bot = TgBot(box_1, cli_1)
    # Start in a separate thread
    thread = threading.Thread(target=tg_bot.run_bot, daemon=True)
    thread.start()
    #Test msg to TG bot
    asyncio.run(tg_bot.send_msg(config.CHAT_ID_TG_BOT, "Welcome Bot"))

    threading.Thread(target=read_central, args=(central_ser, ser, box_list, ), daemon=True).start()

    for port_name, data in ser.items():
        s = data["serial"]
        threading.Thread(target=read_serial_loop, args=(s, box_list, client,), daemon=True).start()

    while True:
        # ENDLESS LOOP
        # Simulation starts here
        if config.SIMULATION == 1:
            #print(ser.items())
            # BOX 1
            #box_1.simulate_box_param()
            #requests.post(box_1.get_url_dev(), box_1.get_packet_str())
            cnt = 0
            #for port_name, data in ser.items():
            #    s = data["serial"]
            #    #if s.in_waiting > 0:  # Se ci sono dati disponibili
            #    val = s.read(config.N_BYTES)
            #    id_, pres, temp, humidity, infr, lock, open_ = struct.unpack('7B', val)
            #    print(
            #        f"ID: {id_}, Presence: {pres}, Temp: {temp}, Humidity: {humidity:}, Infra: {infr}, Lock: {lock}, Open: {open_}")
            #    #if val:
            #    #    id_comm = val[0:3]
            #    #    for b in box_list:
            #    #        #print(f"get_id {b.get_id()}")
            #    #        #print(f"id_comm{id_comm}")
            #    #        if b.get_id() == id_comm:
            #    #            b.set_box_param(val)
            #    #            time.sleep(1)
            #    #            #requests.post(b.get_url_dev(), b.get_packet_str())
            #    #            break
            #    #    #print(val)
            #    #    #sleep(1)

            sleep(0.2)
        else:
            # NO SIMULATION, System is in GO
            pass
            #central_response = central_ser.read(configN_BYTES)
            #if len(central_response) > 1:
            #    if chr(central_response[1]) == CHAR_IDLE:
            #        pass
            #    elif chr(central_response[1]) == CHAR_CHECK:
            #        print("check")
            #        id_acq = central_response[2:5]
            #        for port_name, data in ser.items():
            #            if data["id"] == id_acq:
            #                data["serial"].write(CHAR_UNLOCK)
            #                break
            #    elif chr(central_response[1]) == CHAR_ENROLL:
            #        #enroll
            #        id_acq = central_response[2:5]
            #        print(f"id_acq: {id_acq}")
            #        for port_name, data in ser.items():
            #            if not data["busy"]:
            #                data["id"] = id_acq
            #                print("sto per inviare")
            #                print(data["serial"])
            #                data["serial"].write(id_acq)
            #                data["busy"] = True
            #                #update box_id according with what received
            #                for b in box_list:
            #                    if b.get_id() == "-1":
            #                        b.set_id(id_acq)
            #                        break
            #                break
#
            #    for port_name, data in ser.items():
            #            s = data["serial"]
            #            if s.in_waiting > 0:  # Se ci sono dati disponibili
            #                val = s.read(config.N_BYTES)
            #                if val:
            #                    id_comm = val[0:3]
            #                    for b in box_list:
            #                        if b.get_id() == id_comm:
            #                            b.set_box_param(val)
            #                            requests.post(b.get_url_dev(), b.get_packet_str())
            #                            break
            #                print(val)
            #                sleep(1)

# entry point
if __name__ == '__main__':
    main()
