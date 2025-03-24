"use client";

import mqtt, { MqttClient } from "mqtt";

class MQTT {
  client: MqttClient;
  dataCallback: (data: any) => void;

  constructor(broker: string, port: number, topic: string, dataCallback: (data: any) => void) {
    const clientId = "tchutipon-" + Math.floor(Math.random() * 0xffff).toString();
    const url = `wss://${broker}:${port}/mqtt`;

    this.client = mqtt.connect(url, {
      clientId,
      clean: true,
      connectTimeout: 4000,
    });

    this.dataCallback = dataCallback;

    this.client.on("connect", () => {
      console.log("Connected to MQTT broker");
      this.client.subscribe(topic, (err) => {
        if (err) {
          console.error("Subscription error:", err);
        } else {
          console.log(`Subscribed to topic: ${topic}`);
        }
      });
    });

    this.client.on("message", this.onMessageArrived.bind(this));
    this.client.on("error", (error) => console.error("MQTT Error:", error));
    this.client.on("close", () => console.warn("MQTT Connection Closed"));
  }

  onMessageArrived(topic: string, message: Buffer) {
    try {
      const payload = JSON.parse(message.toString());
      if (payload && payload.axm !== undefined) {
        this.dataCallback(payload);
      }
    } catch (error) {
      console.error("Failed to parse MQTT message:", error);
    }
  }

  disconnect() {
    if (this.client) {
      this.client.end();
      console.log("Disconnected from MQTT broker");
    }
  }
}

export default MQTT;
