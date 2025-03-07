#include <Arduino.h>
#include <ArduinoJSON.h>
#include <Wire.h>
#include <SparkFun_BMI270_Arduino_Library.h>
#include <WiFi.h>
#include <esp_log.h>
#include <PubSubClient.h>

#define TAG "main"

// Initialize Variables
const char* ssid = "EZEKIEL";
const char* password = "late1978";
const char* mqtt_server = "broker.emqx.io";
const char* topic = "ton/server/m5";
const int mqtt_port = 1883;

// Function Declaration
void setup_wifi();
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();

// Initialize WiFi
WiFiClient espClient;
PubSubClient client(espClient);

// Initialize Sensor
BMI270 m5IMU;
uint8_t i2cAddress = BMI2_I2C_SEC_ADDR; // 0x69

// Create Data
struct IMUData{
  float accelX;
  float accelY;
  float accelZ;
  float gyroX;
  float gyroY;
  float gyroZ;
};

// Create Data
IMUData data_buf[5];
int data_count = 0;

void setup() {
  // Begin Serial
  Serial.begin(115200);

  // Start IMU
  pinMode(46, OUTPUT); // Set pin 46 as output
  digitalWrite(46, HIGH); // Set pin 46 to HIGH
  Wire.begin(8, 10, 400000); // Initialize I2C communication with SDA pin 8 and SCL pin 10 at 400kHz
  m5IMU.beginI2C(i2cAddress);

  // Set Up Wifi
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  // client.setCallback(callback);
}
 
void loop() {
  if (!client.connected()){
    reconnect();
  }
  client.loop();

  // Store Data in Buffer
  m5IMU.getSensorData();
  data_buf[data_count].accelX = m5IMU.data.accelX;
  data_buf[data_count].accelY = m5IMU.data.accelY;
  data_buf[data_count].accelZ = m5IMU.data.accelZ;
  data_buf[data_count].gyroX = m5IMU.data.gyroX;
  data_buf[data_count].gyroY = m5IMU.data.gyroY;
  data_buf[data_count].gyroZ = m5IMU.data.gyroZ;

  data_count++;

  if (data_count >= 5) {
    // char buf[] = "{ax:[1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02],ay:[1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02],az:[1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02,1.02]}";
    // Serial.printf("%d",sizeof(buf));
    // Create JSON Object
    // JsonDocument doc;
    // JsonArray axArray = doc.createNestedArray("ax");
    // JsonArray ayArray = doc.createNestedArray("ay");
    // JsonArray azArray = doc.createNestedArray("az");
    // JsonArray gxArray = doc.createNestedArray("gx");
    // JsonArray gyArray = doc.createNestedArray("gy");
    // JsonArray gzArray = doc.createNestedArray("gz");

    // for (int i = 0; i < 10; i++) {
    //   axArray.add(data_buf[i].accelX);
    //   ayArray.add(data_buf[i].accelY);
    //   azArray.add(data_buf[i].accelZ);
    //   gxArray.add(data_buf[i].gyroX);
    //   gyArray.add(data_buf[i].gyroY);
    //   gzArray.add(data_buf[i].gyroZ);
    // }

    char buf[256]; // Make sure the buffer is large enough
    snprintf(buf, sizeof(buf),
             "{\"axm\":[%.2f,%.2f,%.2f,%.2f,%.2f],"
             "\"aym\":[%.2f,%.2f,%.2f,%.2f,%.2f],"
             "\"azm\":[%.2f,%.2f,%.2f,%.2f,%.2f],"
             "\"gxm\":[%.2f,%.2f,%.2f,%.2f,%.2f],"
             "\"gym\":[%.2f,%.2f,%.2f,%.2f,%.2f],"
             "\"gzm\":[%.2f,%.2f,%.2f,%.2f,%.2f]}",
             data_buf[0].accelX, data_buf[1].accelX, data_buf[2].accelX, data_buf[3].accelX, data_buf[4].accelX,
             data_buf[0].accelY, data_buf[1].accelY, data_buf[2].accelY, data_buf[3].accelY, data_buf[4].accelY,
             data_buf[0].accelZ, data_buf[1].accelZ, data_buf[2].accelZ, data_buf[3].accelZ, data_buf[4].accelZ,
             data_buf[0].gyroX, data_buf[1].gyroX, data_buf[2].gyroX, data_buf[3].gyroX, data_buf[4].gyroX,
             data_buf[0].gyroY, data_buf[1].gyroY, data_buf[2].gyroY, data_buf[3].gyroY, data_buf[4].gyroY,
             data_buf[0].gyroZ, data_buf[1].gyroZ, data_buf[2].gyroZ, data_buf[3].gyroZ, data_buf[4].gyroZ
    );
  // // Create JSON Object
  // StaticJsonDocument<200> doc;
  // doc["accelX"] = m5IMU.data.accelX;
  // doc["accelY"] = m5IMU.data.accelY;
  // doc["accelZ"] = m5IMU.data.accelZ;
  // doc["gyroX"] = m5IMU.data.gyroX;
  // doc["gyroY"] = m5IMU.data.gyroY;
  // doc["gyroZ"] = m5IMU.data.gyroZ;

  // char buf[256];
  // // snprintf(buf, sizeof(buf), "{\"demo\":\"%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\"}", m5IMU.data.accelX, m5IMU.data.accelY, m5IMU.data.accelZ, m5IMU.data.gyroX, m5IMU.data.gyroY, m5IMU.data.gyroZ);
  // serializeJson(doc, buf);

    if (client.publish(topic, buf)) {
      Serial.println("Publish successful!");
      data_count = 0; // Reset buffer
    } else {
      Serial.println("Publish failed...");
      data_count = 0;
    }
  }
  delay(100);
}

// Function To Setup WiFi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// Function To Callback
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("M5StickC-Client")) {
      Serial.println("connected");
      client.subscribe(topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}