#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define ssid "MARK"
#define pass "late1978"
#define host "172.20.10.3"
#define port 8305
#define notify "notify-api.line.me"
#define token "b15ajPr1mhglSe58uxBzngKqgvVxx1DAvaYE5mIlQtS"

void setup_wifi(void);

WiFiClient http_client;
char buf[3][64] = {"", "hello", "lat"};
uint8_t count = 0;

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

  String response;

  // Request GET from Fastapi
  if (http_client.connect(host, port)){
    String api = String("") +
    "GET /" + String(buf[count]) + " HTTP/1.1" + "\r\n" + 
    "Host: " + String(host) + "\r\n" +
    "Connection: close" + "\r\n\r\n";
    http_client.print(api);

    // Send HTTP GET request
    // http_client.println("GET /" + String(buf[count]) + " HTTP/1.1");       
    // http_client.println("Host: " + String(host));
    // http_client.println("Connection: close");
    // http_client.println();  // Blank line between headers and body
  
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

  delay(100);

  WiFiClientSecure notify_client;
  notify_client.setInsecure();
  
  // Requeset POST to Notify -> port 443 Secure HTTPS connection
  if (notify_client.connect(notify, 443)){
    // Send HTTP Post Request
    String message = "message=" + response;
    String request = String("") +
                  "POST /api/notify HTTP/1.1" + "\r\n" +
                  "Host: " + String(notify) + "\r\n" +
                  "Content-Type: application/x-www-form-urlencoded" + "\r\n" +
                  "Content-Length: " + String(message.length()) + "\r\n" +
                  "Authorization: Bearer " + String(token) + "\r\n\r\n" + // Blank line between headers and body
                  message + "\r\n";
    notify_client.print(request);

    // Wait for the response
    while (notify_client.connected()){
      String header = notify_client.readStringUntil('\n');
      if (header == "\r"){
        break;
      }
    }

    // Read response body
    String body = notify_client.readString();
    notify_client.stop();
  }

  count ++;
  if (count > 2){
    count = 0;
  }

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