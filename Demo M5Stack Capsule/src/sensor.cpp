#include "main.h"
#include "sensor.h"
#include "comm.h"

// Initialize Sensor
BMI270 m5IMU;
uint8_t i2cAddress = BMI2_I2C_SEC_ADDR; // 0x69
float ax, ay, az, gx, gy, gz;

void sensor_init(void) {
  Wire.begin(8, 10, 400000);
  m5IMU.beginI2C(i2cAddress);
}

void sensor_task(void *pvParam) {
  sensor_init();
  TickType_t xLastWakeTime;
  const TickType_t xFrequency = 100;
  xLastWakeTime = xTaskGetTickCount();
  while(1) {
    m5IMU.getSensorData();
    SensorData data;
    data.ax = m5IMU.data.accelX;
    data.ay = m5IMU.data.accelY;
    data.az = m5IMU.data.accelZ;
    data.gx = m5IMU.data.gyroX;
    data.gy = m5IMU.data.gyroY;
    data.gz = m5IMU.data.gyroZ;

    xQueueSend(queue, &data, 0);
    xTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}