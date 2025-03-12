#include <Arduino.h>
#include <ArduinoJson.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <HTTPClient.h>

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"  // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

#define ssid "EZEKIEL"
#define pass "late1978"
#define mqtt_broker "broker.emqx.io"
// #define mqtt_broker "homeassistant.local"
#define mqtt_port 1883
#define config "tensai"
#define post_topic "ton/server/devices"
#define get_topic "ton/server/get"

void setup_wifi(void);
void setup_mqtt(void);
void callback_mqtt(char* topic, byte* payload, unsigned int length);
void reconnect_mqtt(void);

WiFiClient espclient;
PubSubClient client(espclient);

int scanTime = 5;  //In seconds
BLEScan *pBLEScan;
bool sent = false;
uint8_t dev_count = 0;
uint8_t neg_count = 0;

int temp;
uint8_t not_found = 0;

char buf[128];
char pub[128];
JsonDocument doc;

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    std::string name = advertisedDevice.getName().c_str();
    int rssi = advertisedDevice.getRSSI();
    if (name.find("M5") != std::string::npos){
      BLEDevice::getScan()->stop();
      not_found = 0;
      Serial.print(name.c_str());
      Serial.print(" ");
      Serial.print(rssi);
      Serial.print(" ");
      if (rssi > -80){
        dev_count += (dev_count > 2) ? 0 : 1;
        neg_count = 0;
        Serial.print("Dev Count = ");
        Serial.println(dev_count);
      } else {
        neg_count += (neg_count > 2) ? 0 : 1;
        dev_count = 0;
        Serial.print("Neg Count = ");
        Serial.println(neg_count);
      }
    } 
  }
};

void setup(void) {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  setup_wifi();
  setup_mqtt();

  // Living Room & Bedroom
  BLEDevice::init("Living Room");
  pBLEScan = BLEDevice::getScan();  //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);  //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value

  doc["room"] = "Living Room";

}

void loop(void) {
  pBLEScan->start(scanTime, false);
  pBLEScan->clearResults();
  while (WiFi.status() != WL_CONNECTED){
    dev_count = 0;
    neg_count = 0;
    not_found = 0;
    setup_wifi();
  }

  if (!client.connected()){
    dev_count = 0;
    neg_count = 0;
    not_found = 0;
    reconnect_mqtt();
  }
  client.loop();
  
  if ((dev_count > 0) && (dev_count != temp)){
    temp = dev_count;
    if (!sent){
      sent = true;
      doc["status"] = "in";
      serializeJson(doc, pub);
      client.publish(post_topic, pub);
      Serial.println("Inside room");
    }
  } else if ((neg_count > 0) && (neg_count != temp)){
    temp = neg_count;
    if (sent){
      sent = false;
      doc["status"] = "out";
      serializeJson(doc, pub);
      client.publish(post_topic, pub);
      Serial.print("Outside room");
    }
  } else {
    not_found += (not_found > 2) ? 0 : 1;
    if (not_found > 1){
      Serial.print("Temp Count : ");
      Serial.println(not_found);
      if (sent && (not_found % 3 == 0)){
        sent = false;
        dev_count = 0;
        neg_count = 0;
        doc["status"] = "out";
        serializeJson(doc, pub);
        client.publish(post_topic, pub);
        Serial.println("Outside room");
      }
    }
  }

  delay(2000);
}

// Function to Setup WiFi
void setup_wifi(void){
  Serial.print("Connecting to WiFi ...");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(1000);
  }
}

// Function to Setup MQTT
void setup_mqtt(void){
  client.setServer(mqtt_broker, mqtt_port);
  String clientName = "Alient-" + String(random(0xffff), HEX);
  client.connect(clientName.c_str(), config, config);
  client.setCallback(callback_mqtt);
  client.subscribe(get_topic);
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
}

// Function to Reconnect MQTT Connection
void reconnect_mqtt(void){
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientName = "Alient-" + String(random(0xffff), HEX);
    if (client.connect(clientName.c_str(), config, config)) {
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

