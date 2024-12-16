import serial
import serial.tools.list_ports
import config
from database.firebase_db import FirebaseDB
from mqtt.mqtt import MqttClient

class Initializer:
    def __init__(self):
        self.__ser = None
        self.__firebase = None
        self.__mqtt = None

    def init_system(self):
        self.init_serial_by_os()
        self.init_firebase_db()
        self.init_mqtt_server()

    def init_serial_by_os(self):
        ports = serial.tools.list_ports.comports()
        for p in ports:
            print(p.name)
            if "Arduino" in p.description:
                print ("This is an Arduino!")
                if self.__ser is None:
                    self.__ser = serial.Serial(p.name, config.SERIAL_BAUDRATE)
        print("No Arduino Found")
        print("Going in simulation mode")
        config.SIMULATION = 1;
        self.__ser = None

    def init_firebase_db(self):
        if self.__firebase is None:
            self.__firebase = FirebaseDB()

    def init_mqtt_server(self):
        if self.__mqtt is None:
            self.__mqtt = MqttClient(config.BROKER, config.PORT, config.TOPIC)

    def get_serial(self):
        return self.__ser

    def get_firebase_db(self):
        return self.__firebase

    def get_mqtt(self):
        return self.__mqtt


