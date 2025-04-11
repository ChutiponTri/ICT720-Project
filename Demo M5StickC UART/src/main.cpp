#include <M5StickC.h>

String send_buf = "";
String receive_buf = "";
float ax, ay, az, gx, gy, gz;
uint32_t prev_tim, curr_tim;

void setup(void) {
  // put your setup code here, to run once:
  M5.begin();
  M5.Imu.Init();
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 0, 26);

  M5.Lcd.setRotation(3);
  M5.Lcd.setTextColor(BLUE);
  M5.Lcd.setCursor(30, 30, 4);
  M5.Lcd.printf("M5StickC");

  prev_tim = millis();
  curr_tim = millis();
}

void loop(void) {
  curr_tim = millis();
  if (curr_tim - prev_tim >= 2000) {
    M5.Imu.getAccelData(&ax, &ay, &az);

    int scaled_az = (int)(abs(az) * 100); 
    Serial2.println(scaled_az);
    
    Serial.printf("Read Data: %.2f -> %d\n", az, scaled_az);
    prev_tim = curr_tim;
  }
  if (Serial.available()) { 
    while (Serial.available() > 0) {
      int ch = Serial.read();
      send_buf += (char)ch;
    }
    Serial2.println(send_buf.c_str());
    send_buf = "";
  }

  if (Serial2.available() > 0) {
    while (Serial2.available() > 0) {
      int ch = Serial2.read();
      receive_buf += (char)ch;
    }
    Serial.print("Get Data: ");
    Serial.println(receive_buf.c_str());
    receive_buf = "";
  }

}
