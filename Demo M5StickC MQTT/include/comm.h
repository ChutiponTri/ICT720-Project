#ifndef __COMM_H__
#define __COMM_H__

// Define
#define ssid  "EZEKIEL"
#define password "late1978"
#define mqtt_server "broker.emqx.io"
#define mqtt_topic "ton/server/m5"
#define mqtt_sub "ton/server/get"
#define mqtt_port 1883

// Global Variables
extern WiFiClient espClient;
extern PubSubClient client;

// Global Functions
void comm_init(void);
void comm_task(void *pvParam);
void setup_wifi(void);
void setup_mqtt(void);
void callback(char* topic, byte* payload, unsigned int length);
void reconnect(void);

#endif