#ifndef __COMM_H__
#define __COMM_H__

// Include
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

// Definition
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"  // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

// Structure
struct IMUData {
  float ax, ay, az, gx, gy, gz;
};

// Function Declaration
void comm_init(void);
void comm_task(void *pvParam);

// Global Variable
extern BLEServer *pServer;
extern BLECharacteristic *pTxCharacteristic;
extern bool deviceConnected;
extern bool oldDeviceConnected;
extern uint8_t uuid[6];
extern char addr[18];
extern IMUData data;
extern uint8_t connection_count;

// Class Definition
class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    deviceConnected = true;
    connection_count ++;
    Serial.print("Connected : ");
    Serial.println(connection_count);
    if (connection_count < 2){
    Serial.println("Keep Advertising");
    BLEDevice::startAdvertising();
    }
  };
  
  void onDisconnect(BLEServer *pServer) {
    BLEDevice::startAdvertising();
    connection_count --;
    Serial.print("Disconnected : ");
    Serial.println(connection_count);
    if (connection_count == 0){
      Serial.println("Reset All");
      deviceConnected = false;
    }
  };
};
  
class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0) {
      Serial.println("*********");
      Serial.print("Received Value: ");
      for (int i = 0; i < rxValue.length(); i++) {
        Serial.print(rxValue[i]);
      }

      Serial.println();
      Serial.println("*********");
    }
  }
};

#endif