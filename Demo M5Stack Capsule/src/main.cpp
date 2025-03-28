#include "main.h"
#include "sensor.h"
#include "comm.h"
#include "ble.h"

QueueHandle_t queue;

void setup(void) {
  Serial.begin(115200);
  pinMode(46, OUTPUT);
  digitalWrite(46, HIGH);
  setup_wifi();

  queue = xQueueCreate(10, sizeof(uint8_t));
  
  xTaskCreate(sensor_task, "SENSOR_TASK", 2048, NULL, 3, NULL);
  xTaskCreate(comm_task, "COMM_TASK", 4096, NULL, 2, NULL);
  xTaskCreate(ble_task, "BLE_TASK", 8192, NULL, 1, NULL);
}

void loop(void) {
  if (!client.connected()){
    reconnect();
  }
  client.loop();
  Serial.println("MQTT Connection Running");
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}