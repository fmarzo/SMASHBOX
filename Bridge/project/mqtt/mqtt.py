import asyncio
from time import sleep
from urllib.parse import ResultBase

import paho.mqtt.client as mqtt
import json
import config
from config import CHAT_ID_TG_BOT
from google.api_core.operations_v1.operations_client_config import config

import init
from bot.tgbot import TgBot

from project.config import CHAR_INFR


#bridge rebase
class MqttClient:
    def __init__(self, broker, port, topic):
        self.broker = broker
        self.port = port
        self.topic = topic
        self.client = mqtt.Client()
        # Assign callback functions
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message
        self.__initialized = False

    def start_mqtt(self):
        print(f"Connecting to broker {self.broker}...")
        self.client.connect(self.broker, self.port, keepalive=60)
        self.client.loop_start()

    def stop_mqtt(self):
        print(f"Stopping  broker {self.broker}...")
        self.client.loop_stop()
        self.client.disconnect()

    # Callback function when the client connects to the broker
    def on_connect(self, client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
            # Subscribe to the topic after connecting
            client.subscribe(self.topic)
            print("SUBSCRIBED!")
            self.__initialized = True
        else:
            print(f"Failed to connect, return code {rc}")

    # Callback function when a message is received
    def on_message(self, client, userdata, msg):
        data = json.loads(msg.payload.decode())
        ser = init.Initializer()
        ser_central = ser.get_central_serial()


        tg_bot = TgBot.get_instance()
        asyncio.run(tg_bot.send_msg(CHAT_ID_TG_BOT, "ALARM!"))

        print("WARNING! ALARM RECEIVED!")
        if ser_central is not None:
            ser_central.write(b"_") #sending "_" to notify safe mode to central
            #TODO: send to acquisition notification for locking
        for port_name, data in ser.get_serials().items():
            s = data["serial"]
            s.write(CHAR_INFR) #sending "*" to notify infringement to acquisition



    def is_init (self):
        return self.__initialized


