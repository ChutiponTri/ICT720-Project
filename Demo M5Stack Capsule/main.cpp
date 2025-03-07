#include "main.h"
#include "sensor.h"
#include "comm.h"


void setup() {
  // Begin Serial
  Serial.begin(115200);

  // Create Queue
  queue = xQueueCreate(10, sizeof(uint16_t));

  // Begin FreeRTOS
  xTaskCreate(sensor_task, "SENSOR", 2048, NULL, 2, NULL);
  xTaskCreate(comm_task, "COMM", 2048, NULL, 1, NULL);

}

void loop() {
    vTaskDelay(1000);
}

