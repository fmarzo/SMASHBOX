import paho.mqtt.client as mqtt

# Define the MQTT broker details
HIVE = "broker.hivemq.com"
HIVE_CLOUD = "8146fb450c144928b59c39f764245c06.s1.eu.hivemq.cloud"
BROKER = "test.mosquitto.org"  # Free MQTT broker
PORT = 1883                   # Default MQTT port (non-secure)
HIVE_CLOUD_PORT = 8883
USER = "fmarzo"
PASS = "MyToken1"
TOPIC = "test/topic"          # Topic to publish/subscribe
CLIENT_ID = "PythonMQTTClient"  # Unique client ID

# Callback function when the client connects to the broker
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT Broker!")
        # Subscribe to the topic after connecting
        #client.subscribe(TOPIC)
    else:
        print(f"Failed to connect, return code {rc}")

# Callback function when a message is received
def on_message(client, userdata, msg):
    print(f"Received message from topic '{msg.topic}': {msg.payload.decode()}")

# Create MQTT client instance
client = mqtt.Client(client_id=CLIENT_ID)
#client.username_pw_set(USER, PASS)

# Assign callback functions
client.on_connect = on_connect
client.on_message = on_message

try:
    # Connect to the MQTT broker
    print(f"Connecting to broker {BROKER}...")
    client.connect(BROKER, PORT, keepalive=60)

    # Start the loop to process network traffic and dispatch callbacks
    client.loop_start()


    # Publish a test message
    client.publish("MyTopic", "Hello from Python MQTT Client!")
    # Keep the script running to listen for incoming messages
    input("Press Enter to exit...\n")
    client.loop_stop()
    #client.disconnect()

except Exception as e:
    print(f"An error occurred: {e}")