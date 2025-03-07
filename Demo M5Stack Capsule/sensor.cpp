#include "main.h"
#include "sensor.h"

// Initialize Sensor
BMI270 m5IMU;
uint8_t i2cAddress = BMI2_I2C_SEC_ADDR; // 0x69

// Create Queue
QueueHandle_t queue;
float sensorData[6];

// Function To Initialize Sensor
void sensor_init(void){
    // Start IMU
    pinMode(46, OUTPUT); // Set pin 46 as output
    digitalWrite(46, HIGH); // Set pin 46 to HIGH
    Wire.begin(8, 10, 400000); // Initialize I2C communication with SDA pin 8 and SCL pin 10 at 400kHz
    m5IMU.beginI2C(i2cAddress);
}

// Function To Do Sensor Task
void sensor_task(void *pvParam){
    sensor_init();
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 100;
    xLastWakeTime = xTaskGetTickCount();
    while (1){
        m5IMU.getSensorData();
        sensorData[0] = m5IMU.data.accelX;
        sensorData[1] = m5IMU.data.accelY;
        sensorData[2] = m5IMU.data.accelZ;
        sensorData[3] = m5IMU.data.gyroX;
        sensorData[4] = m5IMU.data.gyroY;
        sensorData[5] = m5IMU.data.gyroZ;

        uint32_t ulVar;
        xQueueSend(queue, (void *)ulVar, 0);

        xTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}