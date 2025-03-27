#include "main.h"
#include "sensor.h"
#include "comm.h"
#include "lcd.h"

QueueHandle_t queue;

void setup(void){
  Serial.begin(115200);

  queue = xQueueCreate(10, sizeof(uint8_t));

  xTaskCreate(sensor_task, "SENSOR_TASK", 2048, NULL, 3, NULL);
  xTaskCreate(comm_task, "COMM_TASK", 8192, NULL, 2, NULL);
  xTaskCreate(lcd_task, "LCD_TASK", 2048, NULL, 1, NULL);
}

void loop(void) {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
