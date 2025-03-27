#include "main.h"
#include "sensor.h"
#include "comm.h"

IMUdata data_buf[5];
uint8_t data_count = 0;

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
    M5.Imu.getAccelData(&data_buf[data_count].ax, &data_buf[data_count].ay, &data_buf[data_count].az);
    M5.Imu.getGyroData(&data_buf[data_count].gx, &data_buf[data_count].gy, &data_buf[data_count].gz);

    xQueueSend(queue, &data_count, 0);
    data_count = (data_count == 4) ? 0 : data_count + 1;

    xTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}