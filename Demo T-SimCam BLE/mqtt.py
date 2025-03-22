import paho.mqtt.client as mqtt
import time

class MQTT_Conn():
    def __init__(self):
        self.client = mqtt.Client(callback_api_version=mqtt.CallbackAPIVersion.VERSION2)
        self.client.on_connect = self.on_connect
        self.client.on_disconnect = self.on_disconnect
        self.client.on_publish = self.on_publish
        self.client.on_message = self.on_message
        mqtt_broker = "broker.emqx.io"
        mqtt_port = 1883
        self.client.connect(mqtt_broker, mqtt_port)

    def on_connect(self, client, userdata, connect_flags, reason_code, properties):
        print(f"Connected {reason_code}")
        self.client.subscribe("ton/server/devices")

    def on_disconnect(self, client, userdata, disconnect_flags, reason_code, properties):
        print(f"Disconnected reason {reason_code}")
        self.client.reconnect()

    def on_publish(self, client, userdata, mid, reason_code, properties):
        print(mid, reason_code)

    def on_message(self, client, userdata, message):
        print(f"{message.topic}: {message.payload}")

    def publish(self, topic, message:str):
        self.client.publish(topic, message.encode())

    def run(self):
        try:
            flag = True
            self.client.loop_start()
            while flag:
                time.sleep(1)
        except KeyboardInterrupt:
            flag = False

if __name__ == '__main__':
    client = MQTT_Conn()
    client.run()
