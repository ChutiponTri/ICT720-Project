#include "main.h"
#include "sensor.h"

// Initialize Sensor
BMI270 m5IMU;
uint8_t i2cAddress = BMI2_I2C_SEC_ADDR; // 0x69
IMUdata data_buf[5];

void sensor_init(void) {
  Wire.begin(8, 10, 400000);
  m5IMU.beginI2C(i2cAddress);
}

void sensor_task(void *pvParam) {
  sensor_init();
  TickType_t xLastWakeTime;
  const TickType_t xFrequency = 1000;
  xLastWakeTime = xTaskGetTickCount();
  uint8_t data_count = 0;
  while(1) {
    m5IMU.getSensorData();
    data_buf[data_count].ax = m5IMU.data.accelX;
    data_buf[data_count].ay = m5IMU.data.accelY;
    data_buf[data_count].az = m5IMU.data.accelZ;
    data_buf[data_count].gx = m5IMU.data.gyroX;
    data_buf[data_count].gy = m5IMU.data.gyroY;
    data_buf[data_count].gz = m5IMU.data.gyroZ;

    xQueueSend(queue, &data_count, 0);
    data_count = (data_count == 4) ? 0 : data_count + 1;

    xTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}