#include <M5StickC.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"  // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;

uint64_t uuid;
char addr[18];

typedef struct {
  float ax;
  float ay;
  float az;
  float gx;
  float gy;
  float gz;
} IMUData;

IMUData data;
uint8_t connection_count = 0;

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

void setup(){
  M5.begin();
  M5.Imu.Init();
  Serial.begin(115200);

  // Create the BLE Device
  BLEDevice::init("AIS M5StickC");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);

  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
  if (deviceConnected) {
    char buf[96];
    M5.Imu.getAccelData(&data.ax, &data.ay, &data.az);
    M5.Imu.getGyroData(&data.gx, &data.gy, &data.gz);

    snprintf(buf, sizeof(buf),
      "{\"ax\":%.2f,"
      "\"ay\":%.2f,"
      "\"az\":%.2f,"
      "\"gx\":%.2f,"
      "\"gy\":%.2f,"
      "\"gz\":%.2f}",
      data.ax, data.ay, data.az, data.gx, data.gy, data.gz
    );

    // pTxCharacteristic->setValue(&txValue, 1);
    pTxCharacteristic->setValue(buf);
    pTxCharacteristic->notify();
    delay(10);  // bluetooth stack will go into congestion, if too many packets are sent
  }

  // disconnecting
  if (!deviceConnected && oldDeviceConnected) {
    delay(500);                   // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising();  // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
  }

  // connecting
  if (deviceConnected && !oldDeviceConnected) {
    oldDeviceConnected = deviceConnected;
  }
}
