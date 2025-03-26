#include <M5StickC.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <WiFiMulti.h>

// Initialize WiFi
const char* ssid = "MARK";
const char* password = "late1978";
const char* mqtt_server = "broker.emqx.io";
const char* topic = "ton/server/m5";
const int mqtt_port = 1883;

// Initialize MQTT Client
WiFiClient espClient;
PubSubClient client(espClient);

// Initialize Structure
struct IMUData {
  float accelX;
  float accelY;
  float accelZ;
  float gyroX;
  float gyroY;
  float gyroZ;
};

IMUData data_buf[5];
int data_count = 0;

// Function Declaration
void setup_wifi();
void reconnect();
void send(String message);

// Function to Setup M5StickC 
void setup() {
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

// Function to loop M5StickC
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (M5.BtnA.wasPressed()){
      send("Hello");
  }

  // Example data to send
  M5.Imu.getAccelData(&data_buf[data_count].accelX, &data_buf[data_count].accelY, &data_buf[data_count].accelZ);
  M5.Imu.getGyroData(&data_buf[data_count].gyroX, &data_buf[data_count].gyroY, &data_buf[data_count].gyroZ);
  data_count ++;

  if (data_count >= 5){
    // Create a JSON document
    char buf[256];
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
    
    if (client.publish(topic, buf)) {
      Serial.println("Publish successful!");
      data_count = 0; // Reset buffer
    } else {
      Serial.println("Publish failed...");
      data_count = 0;
    }
  }
  
  // Wait for some time before publishing again
  delay(100);
}

void setup_wifi() {
  delay(10);
  M5.begin();
  M5.Imu.Init();
  Serial.begin(115200);
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

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("M5StickCClient")) {
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

void send(String message) {
  const char* host = "notify-api.line.me";
  const char* token = "7QihIdYqOvZnRjZZHtYDcJpJ1oboaACM5lNZjlbQgFa";
  WiFiClientSecure client;
  Serial.println("Try");
  client.setInsecure();
  if (!client.connect(host, 443)) {
    Serial.println("Connection failed");
    return;
  }
  Serial.println("Connected");
  String query = String("message=") + message;
  String request = String("") + 
                "POST /api/notify HTTP/1.1" + "\r\n" +
                "Host: " + String(host) + "\r\n" +
                "Authorization: Bearer " + String(token) + "\r\n" +
                "Content-Length: " + String(query.length()) + "\r\n" +
                "Content-Type: application/x-www-form-urlencoded" + "\r\n\r\n" +  // Blank line between headers and body
                query + "\r\n";
  client.print(request);
 
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
    if (line == "\r") {
      break;
    }
  }

  String line = client.readStringUntil('\n');
  Serial.println(line);
}
