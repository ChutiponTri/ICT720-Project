#include "main.h"
#include "ble.h"
#include "comm.h"

// BLE Service
BLEUUID serviceUUID(SERVICE_UUID);
BLEUUID serviceRX(CHARACTERISTIC_UUID_RX);
BLEUUID serviceTX(CHARACTERISTIC_UUID_TX);

// BLE Callback
int scanTime = 5;  //In seconds
BLEScan *pBLEScan;
BLEAdvertisedDevice *device = NULL;
BLERemoteCharacteristic *pRemoteCharacteristic;
BLEClient *pClient;
BLERemoteService *pRemoteService;

// BLE Logic
bool tryConnecting = false;
bool connected = false;
std::string dev_name;

// BLE Buffer
IMUdata data_buf[5];
uint8_t count = 0;
char ble_buf[256];
JsonDocument doc;

// Advertise Callback
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    dev_name = advertisedDevice.getName().c_str();
    if ((sizeof(dev_name) > 0) && (dev_name.find(ble_dev) != std::string::npos)){
      device = new BLEAdvertisedDevice(advertisedDevice);
      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
      advertisedDevice.getScan()->stop();
      tryConnecting = true;
    }
  }
};
  
// Connect / Disconnect Callback
class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient *pclient) {
    Serial.println("onConnect");
  }

  void onDisconnect(BLEClient *pclient) {
    connected = false;
    Serial.println("onDisconnect");
  }
};

void ble_init(void) {
  BLEDevice::init("T-SIMCAM");
}

void ble_task(void *pvParam) {
  ble_init();
  while(1) {
    if (!connected){
      ble_scan();
    }
    if (tryConnecting){
      connection();
    }

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// Function to Scan BLE device
void ble_scan(void){
  pBLEScan = BLEDevice::getScan();    //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);      //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);            // less or equal setInterval value

  while (1) {
    Serial.println("Scanning");
    pBLEScan->start(scanTime, false);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    pBLEScan->clearResults();
    if (device != NULL){
      break;
    }
  }
}
  
// Function to Start BLE Connection
void connection(){
  pClient = BLEDevice::createClient();
  pClient->setClientCallbacks(new MyClientCallback());
  pClient->connect(device);
  pClient->setMTU(517);

  // Obtain a reference to the service we are after in the remote BLE server.
  pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    pClient->disconnect();
    return;
  }

  // Obtain a reference to the characteristic in the service of the remote BLE server.
  pRemoteCharacteristic = pRemoteService->getCharacteristic(serviceTX);
  if (pRemoteCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(serviceTX.toString().c_str());
    pClient->disconnect();
    return;
  } 

  // Start Notify
  if (pRemoteCharacteristic->canNotify()) {
    pRemoteCharacteristic->registerForNotify(notifyCallback);
  } else {
    Serial.println("Cannot Notify");
  }

  connected = true;
  tryConnecting = false;
}

// Notify Callback
static void notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify) {
  DeserializationError error = deserializeJson(doc, pData, length);
  if (error) {
    Serial.println(error.c_str());
    return;
  }
  data_buf[count].ax = doc["ax"];
  data_buf[count].ay = doc["ay"];
  data_buf[count].az = doc["az"];
  data_buf[count].gx = doc["gx"];
  data_buf[count].gy = doc["gy"];
  data_buf[count].gz = doc["gz"];

  xQueueSend(queue, &count, 0);
  count = (count == 4) ? 0 : count + 1;

//   deserializeJson(doc, Serial);
//   Serial.println();
}