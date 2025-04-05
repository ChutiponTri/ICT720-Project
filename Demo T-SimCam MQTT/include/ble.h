#ifndef __BLE_H__
#define __BLE_H__

// Include
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"  // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

#define ble_dev "M5"

// Structure
struct IMUdata {
  float ax, ay, az, gx, gy, gz;
};

// Function Declaration
void ble_scan(void);
void connection(void);
static void notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify);
void ble_init(void);
void ble_task(void *pvParam);

// Global Variable
extern IMUdata data_buf[5];

#endif