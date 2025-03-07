#include "main.h"
#include "sensor.h"
#include "comm.h"
#include "fall.h"

WiFiClient espclient;
PubSubClient client(espclient);

// Function to Setup WiFi Connection
void setup_wifi(){
  Serial.print("Connecting to WiFi ...");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }
}

// Function to Setup MQTT Connection
void setup_mqtt(){
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback_mqtt);
  client.subscribe(get_topic);
}

// Function to Reconnect MQTT Connection
void reconnect_mqtt(){
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientName = "Alient-" + String(random(0xffff), HEX);
    if (client.connect(clientName.c_str())) {
      Serial.println("connected");
      client.subscribe(get_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// MQTT Callback
void callback_mqtt(char* topic, byte* payload, unsigned int length){
  if (length >= sizeof(buf)){
    length = sizeof(buf) - 1;
  }

  // Clear the buffer
  memset(buf, 0, sizeof(buf));

  // Copy the payload to the buffer
  memcpy(buf, payload, length);

  Serial.printf("Received: %s\n", buf);

  send(buf);
}

// Function to run Comm Task
void comm_task(void* pvParam){
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = 100;
  while(1){

    if (WiFi.status() != WL_CONNECTED){
      setup_wifi();
    }

    if (!client.connected()){
      reconnect_mqtt();
    }

    client.loop();

    uint32_t ulVar;
    xQueueReceive(sensorQ, &ulVar, portMAX_DELAY);

    if (client.connected()){
      char mqtt_buf[256];
      char mqtt_buf2[256];
      snprintf(mqtt_buf, sizeof(mqtt_buf),
        "{\"ax1\":[%.2f,%.2f,%.2f,%.2f,%.2f],"
        "\"ay1\":[%.2f,%.2f,%.2f,%.2f,%.2f],"
        "\"az1\":[%.2f,%.2f,%.2f,%.2f,%.2f],"
        "\"gx1\":[%.1f,%.1f,%.1f,%.1f,%.1f],"
        "\"gy1\":[%.1f,%.1f,%.1f,%.1f,%.1f],"
        "\"gz1\":[%.1f,%.1f,%.1f,%.1f,%.1f]}",
        data[0].ax, data[1].ax, data[2].ax, data[3].ax, data[4].ax,
        data[0].ay, data[1].ay, data[2].ay, data[3].ay, data[4].ay,
        data[0].az, data[1].az, data[2].az, data[3].az, data[4].az,
        data[0].gx, data[1].gx, data[2].gx, data[3].gx, data[4].gx,
        data[0].gy, data[1].gy, data[2].gy, data[3].gy, data[4].gy,
        data[0].gz, data[1].gz, data[2].gz, data[3].gz, data[4].gz
      );
      snprintf(mqtt_buf2, sizeof(mqtt_buf2),
        "{\"ax1\":[%.2f,%.2f,%.2f,%.2f,%.2f],"
        "\"ay1\":[%.2f,%.2f,%.2f,%.2f,%.2f],"
        "\"az1\":[%.2f,%.2f,%.2f,%.2f,%.2f],"
        "\"gx1\":[%.1f,%.1f,%.1f,%.1f,%.1f],"
        "\"gy1\":[%.1f,%.1f,%.1f,%.1f,%.1f],"
        "\"gz1\":[%.1f,%.1f,%.1f,%.1f,%.1f]}",
        data[5].ax, data[6].ax, data[7].ax, data[8].ax, data[9].ax,
        data[5].ay, data[6].ay, data[7].ay, data[8].ay, data[9].ay,
        data[5].az, data[6].az, data[7].az, data[8].az, data[9].az,
        data[5].gx, data[6].gx, data[7].gx, data[8].gx, data[9].gx,
        data[5].gy, data[6].gy, data[7].gy, data[8].gy, data[9].gy,
        data[5].gz, data[6].gz, data[7].gz, data[8].gz, data[9].gz
      );

      bool publish = client.publish(post_topic, mqtt_buf);
      bool publish2 = client.publish(post_topic, mqtt_buf2);
      if (publish){
        Serial.println("Message Sent");
      } else {
        Serial.println("Cannot Send");
      }
      if (publish2){
        Serial.println("Message Sent");
      } else {
        Serial.println("Cannot Send");
      }
    }
    

    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}

