#ifndef __SENSOR_H__
#define __SENSOR_H__

// Define Variables
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"  // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

// Global Variables
typedef struct {
    float ax;
    float ay;
    float az;
    float gx;
    float gy;
    float gz;
} IMUData;

extern IMUData data[10];
extern int scanTime;  //In seconds
extern bool tryConnecting;
extern bool connected;
extern char buf[256];
extern float ax, ay, az, gx, gy, gz; 

// Function Declaration
void ble_scan();
void connection();
void sensor_task(void* pvParam);

#endif