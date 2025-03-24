import paho.mqtt.client as mqtt
from datetime import datetime
import requests
import time
import json

class MQTT_Conn():
    def __init__(self):
        self.client = mqtt.Client(client_id="ton-mqtt-client", callback_api_version=mqtt.CallbackAPIVersion.VERSION2)
        self.client.on_connect = self.on_connect
        self.client.on_disconnect = self.on_disconnect
        self.client.on_publish = self.on_publish
        self.client.on_message = self.on_message
        mqtt_broker = "broker.emqx.io"
        # mqtt_broker = "192.168.137.1"
        mqtt_port = 1883
        # self.client.username_pw_set("ton-t-sim", "Chayawut16")
        self.client.connect(mqtt_broker, mqtt_port)
        
    def on_connect(self, client, userdata, connect_flags, reason_code, properties):
        print(f"Connected {reason_code}")
        self.client.subscribe("ict720/ton/#")

    def on_disconnect(self, client, userdata, disconnect_flags, reason_code, properties):
        print(f"Disconnected reason {reason_code}")
        self.client.reconnect()

    def on_publish(self, client, userdata, mid, reason_code, properties):
        print(mid, reason_code)

    def on_message(self, client, userdata, message:mqtt.MQTTMessage):
        print(message.topic, ": ", message.payload)
        payload = json.loads(message.payload)
        payload["timestamp"] = datetime.now().isoformat()
        response = requests.post(url="https://ict720-ab644-default-rtdb.asia-southeast1.firebasedatabase.app/taist.json", json=payload)
        if response.status_code == 200:
            print(f"Success {response.text}")
        else:
            print("Failed", response.status_code, response.text)

    def publish(self, topic, message:str):
        self.client.publish(topic, message.encode())
        
    def run(self):
        try:
            flag = True
            self.client.loop_start()
            while flag:
                client.publish("ton/server/m5", json.dumps({"name": "Ton", "age": 23}))
                time.sleep(5)
        except KeyboardInterrupt:
            flag = False
            
if __name__ == '__main__':
    client = MQTT_Conn()
    client.run()