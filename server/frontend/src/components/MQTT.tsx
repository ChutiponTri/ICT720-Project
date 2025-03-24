"use client";

import Paho from 'paho-mqtt'

class MQTT {
  client: Paho.Client;
  dataCallback: (data: any) => void;

  constructor(broker:string, port:number, topic:string, dataCallback: (data: any) => void) {
    const clientId = "tchutipon-" + Math.floor(Math.random() * 0xffff).toString();
    this.client = new Paho.Client(broker, port, "/mqtt", clientId);

    this.client.onMessageArrived = this.onMessageArrived.bind(this);;
    this.client.onConnectionLost = this.onConnectionLost;

    this.dataCallback = dataCallback; // Save the callback

    this.client.connect({
      onSuccess: () => {
        console.log("Connected to MQTT broker");
        this.client.subscribe(topic);
      },
      onFailure: (error) => console.log("Connection failed:", error)
    });
  }

  onMessageArrived(message: Paho.Message) {
    const payload = JSON.parse(message.payloadString);
    if (payload && payload.axm !== undefined) {
      this.dataCallback(payload);
    }
  }

  onConnectionLost(error: Paho.MQTTError) {
    console.error("Connection lost:", error.errorMessage);
  }
}

export default MQTT