#include <M5StickC.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"  // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
// Polar Sense D4DD2A25

BLEUUID serviceUUID(SERVICE_UUID);
BLEUUID serviceRX(CHARACTERISTIC_UUID_RX);
BLEUUID serviceTX(CHARACTERISTIC_UUID_TX);
BLERemoteCharacteristic *pRemoteCharacteristic;

int scanTime = 5;  //In seconds
BLEScan *pBLEScan;
BLEAdvertisedDevice *device = NULL;
bool doConnect = false;
bool connected = false;

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    std::string dev_name = advertisedDevice.getName().c_str();
    if ((dev_name.length() > 0) && (dev_name.find("Polar Sense D4DD2A25") != std::string::npos)){
      device = new BLEAdvertisedDevice(advertisedDevice);
      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
      advertisedDevice.getScan()->stop();
      doConnect = true;
    }
  }
};

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient *pclient) {
    Serial.println("On Connect");
  }

  void onDisconnect(BLEClient *pclient) {
    connected = false;
    Serial.println("On Disconnect");
  }
};

static void notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify) {
  Serial.print("Notify callback for characteristic ");
  Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
  Serial.print(" of data length ");
  Serial.println(length);
  Serial.print("data: ");
  Serial.write(pData, length);
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();  //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);  //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value

  while(true){
    Serial.println("Finding Device");
    pBLEScan->start(10, false);
    delay(1000);
    pBLEScan->clearResults();
    if (device != NULL){
      break;
    }
  }
}

void loop() {
  if (doConnect){
    delay(4000);
    BLEClient *pClient = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(device);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");
    pClient->setMTU(96);  //set client to request maximum MTU from server (default is 23 otherwise)

    delay(2000);

    // Service
    BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      doConnect = false;
      return;
    }

    // UUID RX
    pRemoteCharacteristic = pRemoteService->getCharacteristic(serviceTX);
    if (pRemoteCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(serviceTX.toString().c_str());
      pClient->disconnect();
      doConnect = false;
      return;
    }

    // Read the value of the characteristic.
    if (pRemoteCharacteristic->canRead()) {
      std::string value = pRemoteCharacteristic->readValue();
      Serial.print("The characteristic value was: ");
      Serial.println(value.c_str());
    } else {
      Serial.println("Characteristic value cannot be read");
    }

    // Start Notify
    if (pRemoteCharacteristic->canNotify()) {
      pRemoteCharacteristic->registerForNotify(notifyCallback);
    } else {
      Serial.println("Cannot Notify");
    }

    // pClient->disconnect();

    doConnect = false;

  }
}