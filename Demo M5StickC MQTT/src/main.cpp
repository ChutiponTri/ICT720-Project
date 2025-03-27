#include "main.h"
#include "sensor.h"
#include "comm.h"
#include "lcd.h"
#include "ble.h"

QueueHandle_t queue;

void setup(void) {
  Serial.begin(115200);
  setup_wifi();

  queue = xQueueCreate(10, sizeof(uint8_t));
  xTaskCreate(sensor_task, "SENSOR_TASK", 2048, NULL, 3, NULL);
  xTaskCreate(comm_task, "COMM_TASK", 4096, NULL, 2, NULL);
  xTaskCreate(lcd_task, "LCD_TASK", 2048, NULL, 1, NULL);
  xTaskCreatePinnedToCore(ble_task, "BLE_TASK", 2048, NULL, 3, NULL, 1);
}

void loop(void) {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  vTaskDelay(1000);
}