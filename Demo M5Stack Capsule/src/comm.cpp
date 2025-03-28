#include "main.h"
#include "sensor.h"
#include "comm.h"

WiFiClient espClient;
PubSubClient client(espClient);
String name;

void comm_init(void) {
  name = "M5Capsule-Client-" + String(random(0xffff), HEX);
  setup_mqtt();
}

void comm_task(void *pvParam) {
  comm_init();
  uint8_t data;
  char buf[256];
  while(1) {
    xQueueReceive(queue, &data, portMAX_DELAY);
    if (data == 4) {
      snprintf(buf, sizeof(buf),
        "{\"axm\":[%.2f,%.2f,%.2f,%.2f,%.2f],"
        "\"aym\":[%.2f,%.2f,%.2f,%.2f,%.2f],"
        "\"azm\":[%.2f,%.2f,%.2f,%.2f,%.2f],"
        "\"gxm\":[%.2f,%.2f,%.2f,%.2f,%.2f],"
        "\"gym\":[%.2f,%.2f,%.2f,%.2f,%.2f],"
        "\"gzm\":[%.2f,%.2f,%.2f,%.2f,%.2f]}",
        data_buf[0].ax, data_buf[1].ax, data_buf[2].ax, data_buf[3].ax, data_buf[4].ax,
        data_buf[0].ay, data_buf[1].ay, data_buf[2].ay, data_buf[3].ay, data_buf[4].ay,
        data_buf[0].az, data_buf[1].az, data_buf[2].az, data_buf[3].az, data_buf[4].az,
        data_buf[0].gx, data_buf[1].gx, data_buf[2].gx, data_buf[3].gx, data_buf[4].gx,
        data_buf[0].gy, data_buf[1].gy, data_buf[2].gy, data_buf[3].gy, data_buf[4].gy,
        data_buf[0].gz, data_buf[1].gz, data_buf[2].gz, data_buf[3].gz, data_buf[4].gz
      );
      client.publish(mqtt_topic, buf);
      Serial.println(buf);
    } else {
      Serial.printf("Count = %d, ax:%.2f,ay:%.2f,az:%.2f,gx:%.2f,gy:%.2f,gz:%.2f\n", 
        data, 
        data_buf[data].ax, 
        data_buf[data].ay, 
        data_buf[data].az, 
        data_buf[data].gx, 
        data_buf[data].gy, 
        data_buf[data].gz
      );
    }
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
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    name = "M5Capsule-Client-" + String(random(0xffff), HEX);
    if (client.connect(name.c_str())) {
      Serial.println("connected");
      client.subscribe(mqtt_sub);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
  }
}
