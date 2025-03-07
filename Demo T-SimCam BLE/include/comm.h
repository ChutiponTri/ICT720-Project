#ifndef __COMM_H__
#define __COMM_H__

// Define Variables
#define ssid "MARK"
#define pass "late1978"
#define mqtt_broker "broker.eqmx.io"
#define mqtt_port 1883
#define post_topic "ton/server/post"
#define get_topic "ton/server/get"

// Global Variables
extern WiFiClient espclient;
extern PubSubClient client;

// Function Declaration
void setup_wifi();
void setup_mqtt();
void reconnect_mqtt();
void callback_mqtt(char* topic, byte* payload, unsigned int length);
void comm_task(void* pvParam);

#endif