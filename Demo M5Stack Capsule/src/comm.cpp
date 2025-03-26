#include "main.h"
#include "sensor.h"
#include "comm.h"

WiFiClient espClient;
PubSubClient client(espClient);

void comm_init(void) {
  setup_mqtt();
}

void comm_task(void *pvParam) {
  comm_init();
  while(1) {
    SensorData data;
    xQueueReceive(queue, &data, portMAX_DELAY);

    char buf[96];
    sprintf(buf, 
      "{\"axm\":[%.2f],"
      "\"aym\":[%.2f],"
      "\"azm\":[%.2f],"
      "\"gxm\":[%.2f],"
      "\"gym\":[%.2f],"
      "\"gzm\":[%.2f]}",
      data.ax, data.ay, data.az, data.gx, data.gy, data.gz
    );
    client.publish(mqtt_topic, buf);
    Serial.println(buf);
  }
}

// Function to Setup WiFi
void setup_wifi(void) {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// Function to Setup MQTT
void setup_mqtt(void) {
  String name = "M5Capsule-Client-" + String(random(0xffff), HEX);
  client.setServer(mqtt_server, mqtt_port);
  client.connect(name.c_str());
  client.setCallback(callback);
  client.subscribe(mqtt_sub);
}

// Callback Function for MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// Function to Reconnect MQTT
void reconnect(void) {
  String name = "M5Capsule-Client-" + String(random(0xffff), HEX);
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(name.c_str())) {
      Serial.println("connected");
      client.subscribe(mqtt_sub);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      vTaskDelay(5000);
    }
  }
}