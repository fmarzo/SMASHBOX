import asyncio
import json
import struct
import time
from time import sleep
import threading
from bot.tgbot import TgBot
from models.box import Box
import config
from models.client import Client
from database.firebase_db import FirebaseDB
from init import Initializer
from config import CHAR_UNLOCK, CHAR_IDLE
from config import CODE_UNLOCK, CODE_ENROLL, TB_TOKEN_1, TB_TOKEN_2, CODE_RE_SAFE, CHAR_RE_SAFE


def read_central(central_ser, ser, box_list):
    fb = FirebaseDB()

    while True:
        central_response = central_ser.read(config.CNTR_N_BYTES)
        if len(central_response) > 1:
            print(central_response)
            header, id = struct.unpack('<2B', central_response)
            print(f"header {header}, fingerID {id}")

            if header == CHAR_IDLE:
                pass
            elif header == CODE_UNLOCK:
                print("check")
                fb.add_new_access(box_list["Box"][0],box_list["Customers"][0])
                for port_name, data in ser.items():
                    if data["id"] == id:
                        data["serial"].write(CHAR_UNLOCK)
                        break
            elif header == CODE_RE_SAFE:
                print("re safe")
                for port_name, data in ser.items():
                    data["serial"].write(CHAR_RE_SAFE)

            elif header == CODE_ENROLL:
                print("enroll")
                enr_packet = [id] + [0] * 9
                print(enr_packet)
                for port_name, data in ser.items():
                    if not data["busy"]:
                        data["id"] = id
                        print("sto per inviare")
                        print(data["serial"])
                        data["serial"].write(bytes(enr_packet))
                        data["busy"] = True
                        # update box_id according with what received
                        for b in box_list["Box"]:
                            if b.get_id() == -1:
                                b.set_id(id)
                                break
                        break

def read_serial_loop(s, box_list):
    print("In thread")
    while True:
        if s.in_waiting >= config.N_BYTES:
            val = s.read(config.N_BYTES)
            id_, pres, temp, humidity, infr, lock, open_ = struct.unpack('7B', val)

            for i, (box, token, client) in enumerate(zip(box_list["Box"], box_list["Token"], box_list["Clients"])):
                if box.get_id() == id_:
                    box.set_raw_box_param(val)
                    payload = box.get_packet()
                    client.publish(config.TB_TOPIC, json.dumps(payload))
                    print(
                        f"ID: {id_}, Presence: {pres}, Temp: {temp}, Humidity: {humidity:}, Infra: {infr}, Lock: {lock}, Open: {open_}")
                    time.sleep(1.0)
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
    box_1 = Box(-1, config.URL_DEVICE_1)
    box_2 = Box(-1, config.URL_DEVICE_2)

    cli_1 = Client("Client", "1", 1001, "client1@gmail.com")

    firebase_db.delete_customer(box_1, cli_1)
    firebase_db.insert_new_customer(box_1, cli_1)
    #firebase_db.update_customer(box_1, cli_1, {'Names': 'Client', 'Surname': '1', 'Mail': "client1@bank.com", 'ID': "1"})

    box_list = {"Box": [box_1, box_2], "Token": [TB_TOKEN_1, TB_TOKEN_2], "Clients": [], "Customers": [cli_1]}

    if config.INJECT_CUSTOMERS == 1:
        firebase_db.insert_new_customer(box_1, cli_1)
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

    for token in box_list["Token"]:
        client = mqtt.Client()
        client.username_pw_set(token)
        client.connect("demo.thingsboard.io", 1883, 60)
        box_list["Clients"].append(client)

    # 5. Disconnetti (dopo breve pausa se necessario)
    #client.disconnect()

    #client.loop_stop()
    #client.disconnect()

    mqtt = system.get_mqtt()
    mqtt.start_mqtt()

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

    if central_ser is not None:
        threading.Thread(target=read_central, args=(central_ser, ser, box_list, ), daemon=True).start()

    for port_name, data in ser.items():
        s = data["serial"]
        threading.Thread(target=read_serial_loop, args=(s, box_list,), daemon=True).start()

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
