#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"  // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

#define ssid "MARK"
#define pass "late1978"
#define mqtt_broker "broker.emqx.io"
#define mqtt_port 1883
#define post_topic "ton/server/post"
#define get_topic "ton/server/get"

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

WiFiClient espclient;
PubSubClient client(espclient);

typedef struct {
  float ax;
  float ay;
  float az;
  float gx;
  float gy;
  float gz;
} IMUData;

uint16_t count = 0;
IMUData data[5];
bool is_falling = false;
float falling_count = 0;

void ble_scan(void);
void connection(void);
void setup_wifi(void);
void setup_mqtt(void);
void reconnect_mqtt(void);
void callback_mqtt(char* topic, byte* payload, unsigned int length);
void send(String message);
void falling_check(float az);

// Advertise Callback
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    std::string dev_name = advertisedDevice.getName().c_str();
    if ((sizeof(dev_name) > 0) && (dev_name.find("XIAO YOYO") != std::string::npos)){
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
  client.publish(post_topic, pData, length);
  Serial.write(pData, length);
  Serial.println();

  /*  // Create a StaticJsonDocument with enough capacity
  JsonDocument doc;

  // Parse the JSON data
  DeserializationError error = deserializeJson(doc, pData);
  if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.f_str());
      return;
  }

  // Retrieve values from the JSON object
  float ax = doc["ax"]; 
  float ay = doc["ay"]; 
  float az = doc["az"]; 
  float gx = doc["gx"]; 
  float gy = doc["gy"]; 
  float gz = doc["gz"]; 

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

  count ++;

  if (client.connected() && (count == 5)){
    char mqtt_buf[256];
    snprintf(mqtt_buf, sizeof(mqtt_buf),
      "{\"ax1\":[%.2f,%.2f,%.2f,%.2f,%.2f],"
      "\"ay1\":[%.2f,%.2f,%.2f,%.2f,%.2f],"
      "\"az1\":[%.2f,%.2f,%.2f,%.2f,%.2f],"
      "\"gx1\":[%.2f,%.2f,%.2f,%.2f,%.2f],"
      "\"gy1\":[%.2f,%.2f,%.2f,%.2f,%.2f],"
      "\"gz1\":[%.2f,%.2f,%.2f,%.2f,%.2f]}",
      data[0].ax, data[1].ax, data[2].ax, data[3].ax, data[4].ax,
      data[0].ay, data[1].ay, data[2].ay, data[3].ay, data[4].ay,
      data[0].az, data[1].az, data[2].az, data[3].az, data[4].az,
      data[0].gx, data[1].gx, data[2].gx, data[3].gx, data[4].gx,
      data[0].gy, data[1].gy, data[2].gy, data[3].gy, data[4].gy,
      data[0].gz, data[1].gz, data[2].gz, data[3].gz, data[4].gz
    );
    client.publish(post_topic, mqtt_buf);
    count = 0;
  }*/
}

// Function to Initialize Device
void setup(void) {
  Serial.begin(115200);
  BLEDevice::init("T-SIMCAM");
  setup_wifi();
  setup_mqtt();
}

// Function to Loop Device
void loop(void) {
  if (WiFi.status() != WL_CONNECTED){
    setup_wifi();
  }

  if (!client.connected()){
    reconnect_mqtt();
  }
  client.loop();

  if (!connected){
    ble_scan();
  }

  if (tryConnecting){
    connection();
  }

  if (is_falling && (falling_count == 5)){
    send("Alert Wheelchair has Fallen");
  }

  delay(100);

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

// Function to Setup WiFi Connection
void setup_wifi(){
  Serial.print("Connecting to WiFi ...");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }
}

// Function to Setup MQTT Connection
void setup_mqtt(void){
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback_mqtt);
  client.subscribe(get_topic);
}

// Function to Reconnect MQTT Connection
void reconnect_mqtt(void){
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientName = "Alient-" + String(random(0xffff), HEX);
    if (client.connect(clientName.c_str())) {
      Serial.println("connected");
      client.subscribe(get_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// MQTT Callback
void callback_mqtt(char* topic, byte* payload, unsigned int length){
  if (length >= sizeof(buf)){
    length = sizeof(buf) - 1;
  }

  // Clear the buffer
  memset(buf, 0, sizeof(buf));

  // Copy the payload to the buffer
  memcpy(buf, payload, length);

  Serial.printf("Received: %s\n", buf);

  send(buf);
}

// Function to Request Line API
void send(String message) {
  const char* host = "notify-api.line.me";
  const char* token = "b15ajPr1mhglSe58uxBzngKqgvVxx1DAvaYE5mIlQtS";
  // const char* token = "TtK9sTE06I8itQbl75gFdcwjdYertYmIbQEhTr7V0Mg";
  WiFiClientSecure http_client;
  Serial.println("Try to Notify");
  http_client.setInsecure();
  if (!http_client.connect(host, 443)) {
    Serial.println("Notify Connection failed");
    return;
  }
  Serial.println("Notify Connected");
  String query = String("message=") + message;
  String request = String("") +
                "POST /api/notify HTTP/1.1" + "\r\n" +
                "Host: " + String(host) + "\r\n" + 
                "Authorization: Bearer " + String(token) + "\r\n" +
                "Content-Length: " + String(query.length()) + "\r\n" +
                "Content-Type: application/x-www-form-urlencoded" + "\r\n\r\n" + // Blank line between headers and body
                query + "\r\n";
  http_client.print(request);

  // Wait for the response
  while (http_client.connected()) {
    String line = http_client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }

  // Read response body
  String line = http_client.readStringUntil('\n');
}

// Function to Check Wheelchair Falling Status
void falling_check(float az){
  if ((az > 0.7) || (az < -0.7)){
    is_falling = true;
    falling_count ++;
  } else {
    is_falling = false;
    falling_count = 0;
  }
}