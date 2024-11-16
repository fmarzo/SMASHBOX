import paho.mqtt.client as mqtt

# Define the MQTT broker details
BROKER = "test.mosquitto.org"  # Free MQTT broker
PORT = 1883                   # Default MQTT port (non-secure)
TOPIC = "box/alarm"          # Topic to publish/subscribe

# Callback function when the client connects to the broker
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT Broker!")
        # Subscribe to the topic after connecting
        client.subscribe(TOPIC)
        print("SUBSCRIBED!")
    else:
        print(f"Failed to connect, return code {rc}")

# Callback function when a message is received
def on_message(client, userdata, msg):
    print(f"Received message from topic '{msg.topic}': {msg.payload.decode()}")

# Create MQTT client instance
client = mqtt.Client()

# Assign callback functions
client.on_connect = on_connect
client.on_message = on_message

try:
    # Connect to the MQTT broker
    print(f"Connecting to broker {BROKER}...")
    client.connect(BROKER, PORT, keepalive=60)

    # Start the loop to process network traffic and dispatch callbacks
    client.loop_forever()

except KeyboardInterrupt:
    print("\nDisconnecting from broker...")
    client.disconnect()
except Exception as e:
    print(f"An error occurred: {e}")