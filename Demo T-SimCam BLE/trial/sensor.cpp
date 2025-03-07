#include "main.h"
#include "sensor.h"
#include "comm.h"
#include "fall.h"

BLEUUID serviceUUID(SERVICE_UUID);
BLEUUID serviceRX(CHARACTERISTIC_UUID_RX);
BLEUUID serviceTX(CHARACTERISTIC_UUID_TX);

int scanTime = 5;  //In seconds
BLEScan *pBLEScan;
BLEAdvertisedDevice *device = NULL;
BLERemoteCharacteristic *pRemoteCharacteristic;
bool tryConnecting = false;
bool connected = false;
char buf[256];
float ax, ay, az, gx, gy, gz; 

IMUData data[10];

// Advertise Callback
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    std::string dev_name = advertisedDevice.getName().c_str();
    if ((sizeof(dev_name) > 0) && (dev_name.find("XIAO MON") != std::string::npos)){
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

// Notify Callback
static void notifyCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify) {
  // Create a StaticJsonDocument with enough capacity
  StaticJsonDocument<200> doc;

  // Parse the JSON data
  DeserializationError error = deserializeJson(doc, pData);
  if (error) {
      Serial.write(pData, length);
      Serial.print(" deserializeJson() failed: ");
      Serial.println(error.f_str());
      return;
  }

  // Retrieve values from the JSON object
  ax = doc["ax"]; 
  ay = doc["ay"]; 
  az = doc["az"]; 
  gx = doc["gx"]; 
  gy = doc["gy"]; 
  gz = doc["gz"]; 

  // Print the values
  Serial.print("ax: ");
  Serial.print(ax);
  Serial.print(",ay: ");
  Serial.print(ay);
  Serial.print(",az: ");
  Serial.print(az);
  Serial.print(",gx: ");
  Serial.print(gx);
  Serial.print(",gy: ");
  Serial.print(gy);
  Serial.print(",gz: ");
  Serial.print(gz);
  Serial.println();

  // // Access data
  data[count].ax = ax;
  data[count].ay = ay;
  data[count].az = az;
  data[count].gx = gx;
  data[count].gy = gy;
  data[count].gz = gz;

  falling_check(az);
  
  if (count < 10){
    count ++;
  } else {
    uint32_t ulVar = millis();
    xQueueSend(sensorQ, (void *)&ulVar, 0);
    count = 0;
  }
}

// Function to Scan BLE device
void ble_scan(){
  pBLEScan = BLEDevice::getScan();    //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);      //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);            // less or equal setInterval value

  while (1) {
    Serial.println("Scanning");
    pBLEScan->start(scanTime, false);
    delay(100);
    pBLEScan->clearResults();
    if (device != NULL){
      break;
    }
  }
}

// Function to Start BLE Connection
void connection(){
    BLEClient *pClient = BLEDevice::createClient();
    pClient->setClientCallbacks(new MyClientCallback());
    pClient->connect(device);
    pClient->setMTU(517);

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
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

void sensor_task(void* pvParam){
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = 1000;
  while(1){
      if (!connected){
      ble_scan();
    }

    if (tryConnecting){
      connection();
    }

    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}