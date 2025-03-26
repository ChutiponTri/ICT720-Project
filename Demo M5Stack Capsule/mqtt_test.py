import paho.mqtt.client as mqtt
import time
import json

class MQTT_Connect():
    def __init__(self):
        super().__init__()
        # Create Variables
        self.client = mqtt.Client(callback_api_version=mqtt.CallbackAPIVersion.VERSION2)
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message
        mqtt_server = "broker.emqx.io"
        mqtt_port = 1883
        self.topic = "ton/server/#"
        self.client.connect(mqtt_server, mqtt_port)

    def on_connect(self, client, userdata, connect_flags, reason_code, properties):
        print("Connected", reason_code)
        self.client.subscribe(self.topic)

    def on_message(self, client, userdata, message):
        data = message.payload
        print(f"{message.topic}: {message.payload}")

    def run(self):
        print("Start")
        try:
            trigger = True
            self.client.loop_start()
            while trigger:
                time.sleep(5)
            self.client.loop_stop()
        except KeyboardInterrupt:
            trigger = False

    def stop(self):
        print("Stop")
        self.client.loop_stop()

if __name__ == '__main__':
    mqtt_client = MQTT_Connect()
    mqtt_client.run()
