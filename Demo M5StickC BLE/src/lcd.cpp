#include "main.h"
#include "sensor.h"
#include "lcd.h"

void lcd_init(void) {
  M5.Lcd.fillScreen(BLACK);
}

void lcd_task(void *pvParam) {
  lcd_init();
  TickType_t xLastWakeTime;
  const TickType_t xFrequency = 1000;
  xLastWakeTime = xTaskGetTickCount();
  while(1) {
    M5.Lcd.fillScreen(BLACK);

    M5.Lcd.setCursor(0, 10);
    M5.Lcd.print(devName);

    M5.Lcd.setCursor(10, 30);
    M5.Lcd.printf("ax: %.2f", data_buf.ax);
    M5.Lcd.setCursor(90, 30);
    M5.Lcd.printf("gx: %.2f", data_buf.gx);

    M5.Lcd.setCursor(10, 40);
    M5.Lcd.printf("ay: %.2f", data_buf.ay);
    M5.Lcd.setCursor(90, 40);
    M5.Lcd.printf("gy: %.2f", data_buf.gy);

    M5.Lcd.setCursor(10, 50);
    M5.Lcd.printf("az: %.2f", data_buf.az);
    M5.Lcd.setCursor(90, 50);
    M5.Lcd.printf("gz: %.2f", data_buf.gz);

    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}