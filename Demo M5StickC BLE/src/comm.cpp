#include "main.h"
#include "sensor.h"
#include "comm.h"

BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;

uint8_t uuid[6];
char addr[18];

IMUData data;
uint8_t connection_count = 0;

void comm_init(void) {
  esp_read_mac(uuid, ESP_MAC_BT);
  snprintf(addr, sizeof(addr), "%02X:%02X:%02X:%02X:%02X:%02X",
    uuid[0], uuid[1], uuid[2], uuid[3], uuid[4], uuid[5]
  );

  std::string dev = std::string(devName);
  BLEDevice::init(dev);
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  pTxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);
  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);
  pRxCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);                 // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}

void comm_task(void *pvParam) {
  comm_init();
  uint8_t data;
  char buf[96];
  while(1) {
    xQueueReceive(queue, &data, portMAX_DELAY);

    Serial.printf("Count: %d ax:%.2f,ay:%.2f,az:%.2f,gx:%.2f,gy:%.2f,gz:%.2f\n",
      data, data_buf.ax, data_buf.ay, data_buf.az,
      data_buf.gx, data_buf.gy, data_buf.gz
    );

    if (deviceConnected) {
      snprintf(buf, sizeof(buf),
        "{\"ax\":%.2f,"
        "\"ay\":%.2f,"
        "\"az\":%.2f,"
        "\"gx\":%.2f,"
        "\"gy\":%.2f,"
        "\"gz\":%.2f}",
        data_buf.ax, data_buf.ay, data_buf.az, data_buf.gx, data_buf.gy, data_buf.gz
      );

      // pTxCharacteristic->setValue(&txValue, 1);
      pTxCharacteristic->setValue(buf);
      pTxCharacteristic->notify();
      vTaskDelay(10 / portTICK_PERIOD_MS);          // bluetooth stack will go into congestion, if too many packets are sent
    }

    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
      vTaskDelay(500 / portTICK_PERIOD_MS);         // give the bluetooth stack the chance to get things ready
      pServer->startAdvertising();                  // restart advertising
      Serial.println("start advertising");
      oldDeviceConnected = deviceConnected;
    }

    // connecting
    if (deviceConnected && !oldDeviceConnected) {
      oldDeviceConnected = deviceConnected;
    }
  }
}