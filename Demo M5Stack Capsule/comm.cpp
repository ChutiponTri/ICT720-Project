#include "main.h"
#include "sensor.h"
#include "comm.h"

// Initialize Variables
const char* ssid = "MARK";
const char* password = "late1978";
const char* mqtt_server = "broker.hivemq.com";
const char* topic = "ton/server";
const int mqtt_port = 1883;

// Initialize WiFi
WiFiClient espClient;
PubSubClient client(espClient);

// Define Functions
void setup_wifi();
void reconnect();

// Function To Initialize COMM
void comm_init(void){
    // Set Up Wifi
    setup_wifi();
    client.setServer(mqtt_server, mqtt_port);
}

// Function TO Do COMM
void comm_task(void *pvParam){
    comm_init();
    while(1){
        if (!client.connected()){
            reconnect();
        }
        client.loop();
        uint32_t ulVar;
        xQueueReceive(queue, &ulVar, portMAX_DELAY);

        char buf[64];
        snprintf(buf, sizeof(buf), "{\"demo\":\"%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\"}", 
        sensorData[0], sensorData[1], sensorData[2], 
        sensorData[3], sensorData[4], sensorData[5]);
        client.publish(topic, buf);

    }
}


void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("M5StickC-Client")) {
      client.subscribe(topic);
    } else {
      delay(5000);
    }
  }
}