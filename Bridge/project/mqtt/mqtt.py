from urllib.parse import ResultBase

import paho.mqtt.client as mqtt
import json
import init

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
        else:
            print(f"Failed to connect, return code {rc}")

    # Callback function when a message is received
    def on_message(self, client, userdata, msg):
        data = json.loads(msg.payload.decode())
        ser = init.Initializer()
        var = ser.get_serials()[data["ID"]]
        print(var)
        print("WARNING! INFRINGEMENT RECEIVED!")
        # TODO: NOTIFY ONLY CENTRAL!
        #for key, value in data.items():
        #    if key == 'ID':
        #        serial_id = value
        #    if key == 'infringement':
        #        if value == 1:
        #            print(serial_id)
        #            result = [entry for entry in ser.get_serials() if entry.get("ID") == serial_id]
        #            print(result)
        #            print("WARNING! INFRINGEMENT RECEIVED!")

