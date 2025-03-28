#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define ssid "EZEKIEL"
#define pass "late1978"
#define host "192.168.137.1"
#define port 8305
#define notify "notify-api.line.me"
#define token "b15ajPr1mhglSe58uxBzngKqgvVxx1DAvaYE5mIlQtS"

void setup_wifi(void);
void get_request(void);
void post_request(void);

WiFiClient http_client;
WiFiClientSecure notify_client;

char buf[3][64] = {"", "hello", "lat"};
uint8_t count = 0;
String response, api;
String message, request, header, body;

// Function to Initialize Device
void setup(void){
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  setup_wifi();
}

// Function to Loop Operations
void loop(void){
  if (WiFi.status() != WL_CONNECTED){
    setup_wifi();
  }

  // GET from FastAPI and Send Notify
  get_request();
  delay(100);
  post_request();

  count = (count > 2) ? 0 : count + 1;

  delay(2000);
}

// Function to Setup WiFi
void setup_wifi(void){
  Serial.print("Connecting WiFi .");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(1000);
  }
  Serial.println();
  Serial.println("WiFi Connected");
}

// Function to GET Data from FastAPI
void get_request(void) {
  if (http_client.connect(host, port)){
    api = String("") +
      "GET /" + String(buf[count]) + " HTTP/1.1" + "\r\n" + 
      "Host: " + String(host) + "\r\n" +
      "Connection: close" + "\r\n\r\n";
    http_client.print(api);
  
    // Wait for the response
    while (http_client.connected()){
      String line = http_client.readStringUntil('\n');
      if (line == "\r") {
        break;
      }
    }
  
    // Read response body
    response = http_client.readString();
    Serial.print("Response : ");
    Serial.println(response);

    http_client.stop();
  }
}

// Function to Post Data to Notify
void post_request(void) {
  notify_client.setInsecure();

  // Requeset POST to Notify -> port 443 Secure HTTPS connection
  if (notify_client.connect(notify, 443)){
    // Send HTTP Post Request
    message = "message=" + response;
    request = String("") +
      "POST /api/notify HTTP/1.1" + "\r\n" +
      "Host: " + String(notify) + "\r\n" +
      "Content-Type: application/x-www-form-urlencoded" + "\r\n" +
      "Content-Length: " + String(message.length()) + "\r\n" +
      "Authorization: Bearer " + String(token) + "\r\n\r\n" + // Blank line between headers and body
      message + "\r\n";
    notify_client.print(request);

    // Wait for the response
    while (notify_client.connected()){
      header = notify_client.readStringUntil('\n');
      if (header == "\r"){
        break;
      }
    }

    // Read response body
    body = notify_client.readString();
    notify_client.stop();
  }
}