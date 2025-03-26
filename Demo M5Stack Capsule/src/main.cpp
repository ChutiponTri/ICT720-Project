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

  queue = xQueueCreate(10, sizeof(SensorData));
  xTaskCreate(sensor_task, "SENSOR_TASK", 4096, NULL, 3, NULL);
  xTaskCreate(comm_task, "COMM_TASK", 4096, NULL, 2, NULL);
  xTaskCreatePinnedToCore(ble_task, "BLE_TASK", 4096, NULL, 3, NULL, 1);
}

void loop(void) {
  if (!client.connected()){
    reconnect();
  }
  client.loop();
  vTaskDelay(1000);
}