#ifndef __SENSOR_H__
#define __SENSOR_H__

// Glabal Variables
extern QueueHandle_t queue;
extern float ax, ay, az, gx, gy, gz;

// Global Functions
void sensor_init(void);
void sensor_task(void *pvParam);

#endif