#include <M5StickC.h>
#include "sensor.h"

// global variable
QueueHandle_t sensorQ;
float ax,ay,az,gx,gy,gz;

// initialize sensor
void sensor_init(void) {
    M5.imu.Init();
}

// read sensor, comput ???
void sensor_task(void *pvParameters) {
    sensor_init();
    TickType_t xLastWaketime;
    const TickType_t xFrequency = 10;
    xLastWaketime = xTaskGetTickCount();
    while(1) {
        M5.Imu.getAccelData(&ax,&ay,&az);
        M5.Imu.getGyroData(&gx,&gy,&gz);
        // Serial.print("Sensor task :");
        // Serial.println(millis());
        // Serial.printf("%.2f %.2f %.2f %.2f %.2f %.2f\n", ax,ay,az,gx,gy,gz);


        uint32_t ulVar = millis();
        xQueueSend(sensorQ, (void * ) &ulVar, 0 );

        vTaskDelayUntil( &xLastWaketime, xFrequency );
        m5.update();
    }
}