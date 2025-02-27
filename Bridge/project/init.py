import os

import serial
import serial.tools.list_ports
from scipy.stats import false_discovery_control

import config
from database.firebase_db import FirebaseDB
from mqtt.mqtt import MqttClient
from time import sleep

class Initializer:
    _instance = None
    _initialized = False
    def __new__(cls, *args, **kwargs):
        if cls._instance is None:
            cls._instance = super().__new__(cls)
        return cls._instance

    def __init__(self):
        if not self._initialized:
            Initializer._initialized = True
            self.__ser_ports_dict = {}
            self.__ser_central = None
            self.__firebase = None
            self.__mqtt = None
            self.__tg_bot = None
        sleep(1)

    def init_system(self):
        self.init_serial_by_os()
        self.init_firebase_db()
        self.init_mqtt_server()
        #self.init_tg_bot()

    def init_serial_by_os(self):
        serial_found = 0
        ports = serial.tools.list_ports.comports()
        print(os.name)
        descr = "Arduino"
        if os.name == config.WINDOWS_OS_ID:
            # in a Win env
            for p in ports:
                print(p.name)
                if descr in p.description:
                    print("This is an Arduino!")
                    # append it as more than one Arduino can be found
                    self.__ser_ports_dict[p.name] = {
                        "serial": serial.Serial(p.name, config.SERIAL_BAUDRATE),
                        "busy": False,
                        "id": 0
                    }
                    serial_found = 1
        else:
            #n not in a Win env
            for p in ports:
                print(p.name)
                if "cu.usbmodem" in p.name:
                    print("This is an Arduino!")
                    # append it as more than one Arduino can be found
                    name = f"/dev/{p.name}"
                    self.__ser_ports_dict[name] = {
                        "serial": serial.Serial(name, config.SERIAL_BAUDRATE),
                        "busy": False,
                        "id": 0
                    }
                    serial_found = 1

        sleep(2)

        if serial_found == 0:
            print("No Arduino Found")
            print("Going in simulation mode")
            config.SIMULATION = 1
        else:
            for port_name, data in self.__ser_ports_dict.items():
                ser = data["serial"]
                packet = ser.read(config.N_BYTES)
                id_ser_0 = int(chr(packet[0]))
                id_ser_1 = int(chr(packet[1]))
                id_ser_2 = int(chr(packet[2]))

                id_ser = id_ser_0 + id_ser_1 + id_ser_2

                if id_ser == config.CENTRAL_SERIAL:
                    self.__ser_central = ser
                    print("Central serial assigned: it's " + str(self.__ser_central))
                    self.__ser_central.write(b"@")
                    break
            if self.__ser_central is not None:
                del self.__ser_ports_dict[self.__ser_central.name]
            else:
                #print("Central not found")
                #print("Going in simulation mode")
                config.SIMULATION = 1

    def init_firebase_db(self):
        if self.__firebase is None:
            self.__firebase = FirebaseDB()

    def init_mqtt_server(self):
        if self.__mqtt is None:
            self.__mqtt = MqttClient(config.BROKER, config.PORT, config.TOPIC)

    def init_tg_bot(self):
        if self.__tg_bot is None:
            self.__tg_bot = TgBot()

    def get_serials(self):
        return self.__ser_ports_dict

    def get_central_serial(self):
        return self.__ser_central

    def get_firebase_db(self):
        return self.__firebase

    def get_mqtt(self):
        return self.__mqtt

    def get_tg_bot(self):
        return self.__tg_bot



