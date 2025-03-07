#include <iostream>
#include <stdio.h>

struct IMUData{
    float accelX;
    float accelY;
    float accelZ;
    float gyroX;
    float gyroY;
    float gyroZ;
};

IMUData data_buf;

int main() {
    float data_temp[] = {-0.12, -0.23, -0.33, -0.43, -0.55, 0.56, 0.76, 0.87, 0.99, -101.00};
    for (int i=0; i<10; i++){
        data_buf.accelX = data_temp[i];
        data_buf.accelY = data_temp[i];
        data_buf.accelZ = data_temp[i];
        data_buf.gyroX = data_temp[i];
        data_buf.gyroY = data_temp[i];
        data_buf.gyroZ = data_temp[i];
    }
    char buf[96];

    snprintf(buf, sizeof(buf),
        "{\"ax\":[%.2f],"
         "\"ay\":[%.2f],"
         "\"az\":[%.2f],"
         "\"gx\":[%.2f],"
         "\"gy\":[%.2f],"
         "\"gz\":[%.2f]}",
         data_buf.accelX,
         data_buf.accelY, 
         data_buf.accelZ,
         data_buf.gyroX, 
         data_buf.gyroY, 
         data_buf.gyroZ 
        );

    std::cout << buf << std::endl;
    printf("%d", sizeof(buf));
    return 0;
}
