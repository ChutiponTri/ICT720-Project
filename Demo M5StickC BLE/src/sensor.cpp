#include "main.h"
#include "sensor.h"
#include "comm.h"

IMUdata data_buf;
uint8_t data_count = 0;

void sensor_init(void) {
  M5.begin();
  M5.Imu.Init();
}

void sensor_task(void *pvParam) {
  sensor_init();
  TickType_t xLastWakeTime;
  const TickType_t xFrequency = 100;
  xLastWakeTime = xTaskGetTickCount();
  while(1) {
    M5.Imu.getAccelData(&data_buf.ax, &data_buf.ay, &data_buf.az);
    M5.Imu.getGyroData(&data_buf.gx, &data_buf.gy, &data_buf.gz);

    xQueueSend(queue, &data_count, 0);
    data_count = (data_count == 4) ? 0 : data_count + 1;

    xTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}