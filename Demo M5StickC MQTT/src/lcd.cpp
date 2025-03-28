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
    M5.Lcd.print("TON M5");

    M5.Lcd.setCursor(10, 30);
    M5.Lcd.printf("ax: %.2f", data_buf[data_count].ax);
    M5.Lcd.setCursor(90, 30);
    M5.Lcd.printf("gx: %.2f", data_buf[data_count].gx);

    M5.Lcd.setCursor(10, 40);
    M5.Lcd.printf("ay: %.2f", data_buf[data_count].ay);
    M5.Lcd.setCursor(90, 40);
    M5.Lcd.printf("gy: %.2f", data_buf[data_count].gy);

    M5.Lcd.setCursor(10, 50);
    M5.Lcd.printf("az: %.2f", data_buf[data_count].az);
    M5.Lcd.setCursor(90, 50);
    M5.Lcd.printf("gz: %.2f", data_buf[data_count].gz);

    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}