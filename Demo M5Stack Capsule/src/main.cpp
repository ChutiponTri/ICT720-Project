#include "main.h"
#include "ble.h"
#include "sensor.h"
#include "comm.h"

QueueHandle_t queue;

void setup(void) {
  Serial.begin(115200);
  auto config = M5.config();
  M5.begin(config);

  setup_wifi();
  setup_ble();

  queue = xQueueCreate(10, sizeof(float));
  xTaskCreate(sensor_task, "SENSOR_TASK", 4096, NULL, 3, NULL);
  xTaskCreate(comm_task, "COMM_TASK", 4096, NULL, 2, NULL);
}

void loop(void) {
  if (!client.connected()){
    reconnect();
  }
  client.loop();
  vTaskDelay(1000);
}