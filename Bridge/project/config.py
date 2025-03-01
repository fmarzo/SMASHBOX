"""
========================================================================================================================
File Name:       config.py
Description:     File containing constants and definitions for application
========================================================================================================================
"""
#DEVICE URL

URL_DEVICE_1 = "http://demo.thingsboard.io/api/v1/o0SzxyiH8fZWK1uQrRh2/telemetry"
URL_DEVICE_2 = "http://demo.thingsboard.io/api/v1/7QC2PthzFKhkp9AJAW2z/telemetry"
URL_DEVICE_3 = "http://demo.thingsboard.io/api/v1/wbUjViNjPWcBp2iyHGDy/telemetry"
URL_DEVICE_4 = "http://demo.thingsboard.io/api/v1/7nxoq3udn22iyy2inrwh/telemetry"
URL_DEVICE_5 = "http://demo.thingsboard.io/api/v1/1EB9tCjP9BbUM7IYIqAe/telemetry"

#PARAMETERS SIMULATION INDEXES

LOCK_PARAM = 1
TEMP_PARAM = 10

#SERIAL PARAMETERS

SERIAL_BAUDRATE = 9600
SERIAL_COM_MAC_RADIX = "/dev/cu.usbmodem"
SERIAL_COM_WIN_RADIX = "COM"
N_BYTES = 11
CENTRAL_SERIAL = 0
CHAR_CHECK = "1"
CHAR_ENROLL = "2"
CHAR_UNLOCK = b"&"
CHAR_IDLE = "0"
#FIREBASE PARAMS

DB_DATABASE_URL = "https://customerlist-970ff-default-rtdb.europe-west1.firebasedatabase.app/"
DB_CREDENTIALS_PATH = "credential.json"
DB_FIELD_NAME = 'Name'
DB_FIELD_SURNAME = 'Surname'
DB_FIELD_ID = 'ID'
DB_FIELD_BOX = 'Box'
DB_FIELD_MAIL = 'Mail'
DB_ROOT_PATH = "/Customers"

#MQTT SERVER
BROKER = "broker.hivemq.com"  # Free MQTT broker
PORT = 1883                   # Default MQTT port (non-secure)
TOPIC = "box/alarm"          # Topic to publish/subscribe

#OPERATING SYSTEM ID
WINDOWS_OS_ID = "nt"

#DEBUG_PARAMETERS
SIMULATION = 0 # no simulation for default
INJECT_CUSTOMERS = 0
DELETE_CUSTOMERS = 0
UPDATE_CUSTOMERS = 0

#TELEGRAM BOT
TOKEN_TG_BOT = "8028914955:AAFOlJLWA2L791CFkMvAINPLas-5n3WRpTY"
CHAT_ID_TG_BOT = 2074734224