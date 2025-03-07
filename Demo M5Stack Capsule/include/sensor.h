#ifndef __SENSOR_H__
#define __SENSOR_H__

#include <SparkFun_BMI270_Arduino_Library.h>

// Glabal Variables
extern BMI270 m5IMU;
extern QueueHandle_t queue;
extern float sensorData[6];

// Global Functions
void sensor_init(void);
void sensor_task(void *pvParam);

#endif