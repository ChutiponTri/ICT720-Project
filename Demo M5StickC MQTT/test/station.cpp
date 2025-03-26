#include <M5StickC.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "esp_mac.h"

#define ssid "EZEKIEL"
#define pass "late1978"
#define mqtt_broker "192.168.137.1"
#define mqtt_port 1883
#define mqtt_beat "ict720/ton/station3/beat"
#define mqtt_topic "ict720/ton/station3/data"
#define mqtt_cmd "ict720/ton/station3/cmd"
#define station "M5 Station 3"
#define dev_name "Asset"

// function prototypes
void on_message(char* topic, byte* payload, unsigned int length);
void lcd_write(const char* device);
String getInterfaceMacAddress(esp_mac_type_t interface);

// shared variables
uint32_t scanTime = 3;  
BLEScan *pBLEScan;
WiFiClient espClient;
PubSubClient mqtt_client(espClient);
JsonDocument doc;
String clientName = "M5-Client-" + String(random(0xffff), HEX);
String mac;
String ip;

// callback for BLE scan
class CustomAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
  }
};

void setup() {
  // 0. init M5
  M5.begin();
  M5.Lcd.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextSize(2);
  M5.Lcd.print(station);

  // 1. init WiFi
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());
  Serial.printf("RSSI: %d\n", WiFi.RSSI());
  mac = getInterfaceMacAddress(ESP_MAC_WIFI_STA);
  ip = WiFi.localIP().toString();

  // 2. init BLE scanner
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();  //create new scan
  //pBLEScan->setAdvertisedDeviceCallbacks(new CustomAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);  //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value

  // 3. connect to MQTT broker
  mqtt_client.setServer(mqtt_broker, mqtt_port);
  mqtt_client.setCallback(on_message);
  mqtt_client.connect(clientName.c_str());
  mqtt_client.subscribe(mqtt_cmd);
  Serial.println("Connected to MQTT broker");
}

void loop() {
  char payload[120];
  doc.clear();
  doc["millis"] = millis();
  doc["rssi"] = WiFi.RSSI();
  doc["mac"] = mac.c_str();
  doc["ip"] = ip.c_str();
  serializeJson(doc, payload);
  mqtt_client.publish(mqtt_beat, payload);
  mqtt_client.loop();
  // scan for BLE devices
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.printf("Found %d devices\n", foundDevices.getCount());
  for (int i = 0; i < foundDevices.getCount(); i++) {
    BLEAdvertisedDevice device = foundDevices.getDevice(i);
    if (device.haveName()) {
      if (strncmp(device.getName().c_str(), dev_name, strlen(dev_name)) == 0) {
        const char* name = device.getName().c_str();
        String buf_name = String(name);
        Serial.printf("Found Asset: %s\n", name);
        lcd_write(name);
        doc.clear();
        sprintf(payload, "{\"millis\":%d,\"name\":\"%s\",\"mac\":\"%s\",\"rssi\":%d}", millis(), buf_name.c_str(), device.getAddress().toString().c_str(), device.getRSSI());
        Serial.println(payload);
        mqtt_client.publish(mqtt_topic, payload);
        mqtt_client.loop();
      } else {
        Serial.printf("Found device: %s\n", device.getName().c_str());
      }
    } else {
      Serial.printf("Device without name: %s\n", device.getAddress().toString().c_str());
    }
  }
}

void on_message(char* topic, byte* payload, unsigned int length) {

}

void lcd_write(const char* device) {
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("Found device : %s", device);
}

String getInterfaceMacAddress(esp_mac_type_t interface) {
  String mac = "";
  unsigned char mac_base[6] = {0};

  if (esp_read_mac(mac_base, interface) == ESP_OK) {
    char buffer[18];  // 6*2 characters for hex + 5 characters for colons + 1 character for null terminator
    sprintf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X", mac_base[0], mac_base[1], mac_base[2], mac_base[3], mac_base[4], mac_base[5]);
    mac = buffer;
  }

  return mac;
}