#include "main.h"
#include "sensor.h"
#include "comm.h"

float ax, ay, az, gx, gy, gz;

void sensor_init(void) {
  M5.begin();
  M5.Imu.Init();
  M5.Lcd.begin();
  M5.Lcd.setRotation(3);
}

void sensor_task(void *pvParam) {
  sensor_init();
  TickType_t xLastWakeTime;
  const TickType_t xFrequency = 100;
  xLastWakeTime = xTaskGetTickCount();
  while(1) {
    M5.Imu.getAccelData(&ax, &ay, &az);
    M5.Imu.getGyroData(&gx, &gy, &gz);

    xQueueSend(queue, &ax, 0);
    xTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}