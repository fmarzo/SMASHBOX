import os

import serial
import serial.tools.list_ports
import config
from database.firebase_db import FirebaseDB
from mqtt.mqtt import MqttClient

class Initializer:
    def __init__(self):
        self.__ser_ports_list = []
        self.__firebase = None
        self.__mqtt = None

    def init_system(self):
        self.init_serial_by_os()
        self.init_firebase_db()
        self.init_mqtt_server()

    def init_serial_by_os(self):
        serial_found = 0
        ports = serial.tools.list_ports.comports()
        print(os.name)
        descr = "Arduino"
        if os.name != config.WINDOWS_OS_ID:
            # not in a Win env
            descr = None
        for p in ports:
            print(p.name)
            if descr in p.description:
                print ("This is an Arduino!")
                #append it as more than one Arduino can be found
                self.__ser_ports_list.append(serial.Serial(p.name, config.SERIAL_BAUDRATE))
                serial_found = 1

        if serial_found == 0:
            print("No Arduino Found")
            print("Going in simulation mode")
            config.SIMULATION = 1

    def init_firebase_db(self):
        if self.__firebase is None:
            self.__firebase = FirebaseDB()

    def init_mqtt_server(self):
        if self.__mqtt is None:
            self.__mqtt = MqttClient(config.BROKER, config.PORT, config.TOPIC)

    def get_serials(self):
        return self.__ser_ports_list

    def get_firebase_db(self):
        return self.__firebase

    def get_mqtt(self):
        return self.__mqtt


