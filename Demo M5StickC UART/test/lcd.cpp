#include <M5StickC.h>
#include "lcd.h"
#include "sensor.h"

// initialize lcd
void lcd_init(void) {
    M5.Lcd.setRotation(3);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.printf("Ver: %s %s\n", __DATE__, __TIME__);
    M5.Lcd.print("Device :  UART");
    M5.Lcd.println();
    M5.Lcd.println();
    M5.Lcd.printf("     Accel        Gyro");
    M5.Lcd.println();
    M5.Lcd.printf("X:           X:");
    M5.Lcd.println();
    M5.Lcd.printf("Y:           Y:");
    M5.Lcd.println();
    M5.Lcd.printf("Z:           Z:");
}

// read sensor, comput ???
void lcd_task(void *pvParameters) {
    lcd_init();
    TickType_t xLastWaketime;
    const TickType_t xFrequency = 1000;
    xLastWaketime = xTaskGetTickCount();
    while(1) {
        M5.Lcd.setCursor(8*4, 8*4);
        M5.Lcd.printf("%.2f       %.2f  \n",ax,gx);
        M5.Lcd.setCursor(8*4, 8*5);
        M5.Lcd.printf("%.2f       %.2f  \n",ay,gy);
        M5.Lcd.setCursor(8*4, 8*6);
        M5.Lcd.printf("%.2f       %.2f  \n",az,gz);

        // Serial.print("Lcd task :");
        // Serial.println(millis());

        vTaskDelayUntil( &xLastWaketime, xFrequency );
    }
}