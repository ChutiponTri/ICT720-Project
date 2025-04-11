#include <M5StickC.h>
#include "comm.h"
#include "sensor.h"

char buf[96];

// initialize WiFi and MQTT
void comm_init(void) {

}

// comm using MQTT
void comm_task(void *pvParameters) {
    comm_init();
    while(1) {

        uint32_t ulVar;
        xQueueReceive( sensorQ, &(ulVar), portMAX_DELAY );

        //Serial.print("Comm task :");
        //Serial.println(millis());
        snprintf(buf, sizeof(buf), 
            "{\"ax\":%.2f,"
             "\"ay\":%.2f,"
             "\"az\":%.2f,"
             "\"gx\":%.2f,"
             "\"gy\":%.2f,"
             "\"gz\":%.2f}\n",
            ax, ay, az, gx, gy, gz
        );
        Serial.printf(buf);
    }
}