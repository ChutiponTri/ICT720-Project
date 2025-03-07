#include <Arduino.h>
#include <ArduinoJSON.h>
#include <Wire.h>
#include <SparkFun_BMI270_Arduino_Library.h>
#include <WiFi.h>
#include <esp_log.h>
#include <PubSubClient.h>

#define TAG "main"

// Initialize Variables
const char* ssid = "MARK";
const char* password = "late1978";
const char* mqtt_server = "broker.emqx.io";
const char* topic = "ton/server/one";
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
IMUData data_buf;
int prev_millis = millis();
int next_millis = millis();

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
}

void loop() {
    if (!client.connected()){
    reconnect();
    }
    client.loop();

    next_millis = millis();
    if (next_millis - prev_millis >= 100 ){
        // Store Data in Buffer
        m5IMU.getSensorData();
        data_buf.accelX = m5IMU.data.accelX;
        data_buf.accelY = m5IMU.data.accelY;
        data_buf.accelZ = m5IMU.data.accelZ;
        data_buf.gyroX = m5IMU.data.gyroX;
        data_buf.gyroY = m5IMU.data.gyroY;
        data_buf.gyroZ = m5IMU.data.gyroZ;

        char buf[256]; // Make sure the buffer is large enough
        snprintf(buf, sizeof(buf),
                "{\"ax1\":[%.2f],"
                "\"ay1\":[%.2f],"
                "\"az1\":[%.2f],"
                "\"gx1\":[%.2f],"
                "\"gy1\":[%.2f],"
                "\"gz1\":[%.2f]}",
                data_buf.accelX,
                data_buf.accelY, 
                data_buf.accelZ,
                data_buf.gyroX,
                data_buf.gyroY,
                data_buf.gyroZ
        );

        if (client.publish(topic, buf)) {
            Serial.println("Publish successful!");
            
        } else {
            Serial.println("Publish failed...");
        }
        prev_millis = next_millis;
    }

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