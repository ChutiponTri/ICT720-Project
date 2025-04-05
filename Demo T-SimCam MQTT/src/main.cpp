#include "main.h"
#include "comm.h"
#include "ble.h"

QueueHandle_t queue;

void setup(void) {
  Serial.begin(115200);
  setup_wifi();

  queue = xQueueCreate(10, sizeof(uint8_t));
  xTaskCreate(ble_task, "BLE_TASK", 4096, NULL, 2, NULL);
  xTaskCreate(comm_task, "COMM_TASK", 8192, NULL, 1, NULL);
}

void loop(void) {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  client.publish(mqtt_beat, "beat");

  vTaskDelay(1000 / portTICK_PERIOD_MS);
}