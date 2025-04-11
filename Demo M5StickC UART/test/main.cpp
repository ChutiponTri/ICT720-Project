#include <M5StickC.h>
#include "sensor.h"
#include "comm.h"
#include "lcd.h"

void setup(void) {
  // put your setup code here, to run once:
  m5.begin();
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 0, 26);

  // create tasks
  sensorQ = xQueueCreate(10, sizeof(uint32_t));
  xTaskCreate(sensor_task, "SENSOR", 2048, NULL, 3, NULL); /*/ 2048 = 2Kb per task /*/
  xTaskCreate(comm_task, "COMM", 2048, NULL, 2, NULL);
  xTaskCreate(lcd_task, "LCD", 2048, NULL, 1, NULL);
}

void loop(void) {
  vTaskDelay(5000 / portTICK_PERIOD_MS);
}
