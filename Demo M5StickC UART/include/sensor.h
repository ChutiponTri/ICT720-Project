#ifndef _SENSOR_H_
#define _SENSOR_H_

// global variable
extern QueueHandle_t sensorQ;
extern float ax,ay,az,gx,gy,gz,temp;

// function prototypes
void sensor_init(void);
void sensor_task(void *pvParameters);

#endif /* _SENSOR_H_ */