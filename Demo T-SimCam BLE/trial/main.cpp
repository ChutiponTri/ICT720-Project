#include "main.h"
#include "sensor.h"
#include "comm.h"
#include "fall.h"

QueueHandle_t sensorQ;

// Function to Initialize Device
void setup() {
  Serial.begin(115200);
  BLEDevice::init("T-SIMCAM");
  setup_wifi();
  setup_mqtt();

  sensorQ = xQueueCreate(10, sizeof(uint32_t));
  xTaskCreate(sensor_task, "SENSOR", 2048, NULL, 3, NULL);
  xTaskCreate(comm_task, "COMM", 4096, NULL, 2, NULL);
  xTaskCreate(fall_task, "FALL", 4096, NULL, 1, NULL);
}

// Function to Loop Device
void loop() {

  vTaskDelay(1000);

}
