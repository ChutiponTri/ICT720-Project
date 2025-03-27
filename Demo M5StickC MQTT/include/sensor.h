#ifndef __SENSOR_H__
#define __SENSOR_H__

// Define Structure
struct IMUdata {
  float ax, ay, az, gx, gy, gz;
};

// Glabal Variables
extern QueueHandle_t queue;
extern IMUdata data_buf[5];
extern uint8_t data_count;

// Global Functions
void sensor_init(void);
void sensor_task(void *pvParam);

#endif