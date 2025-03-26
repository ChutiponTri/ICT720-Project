#include "main.h"
#include "ble.h"

void ble_init(void) {
  BLEDevice::init("M5Capsule");
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
}

void ble_task(void *pvParam) {
  ble_init();
  while(1) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // Adjust delay as needed
  }
}