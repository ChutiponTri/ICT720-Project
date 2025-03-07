#include "main.h"
#include "sensor.h"
#include "fall.h"

uint16_t count = 0;
bool is_falling = false;
float falling_count = 0;

// Function to Request Line API
void send(String message) {
  const char* host = "notify-api.line.me";
  // const char* token = "b15ajPr1mhglSe58uxBzngKqgvVxx1DAvaYE5mIlQtS";
  const char* token = "TtK9sTE06I8itQbl75gFdcwjdYertYmIbQEhTr7V0Mg";
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

// Function to run Falling Task
void fall_task(void* pvParm){
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = 100;
  while(1){
    
    if (is_falling && (falling_count == 5)){
      send("Alert Wheelchair has Fallen");
    }

    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}