#ifndef __MAIN_H__
#define __MAIN_H__

// Include Libs
#include <M5StickC.h>
#include <esp_log.h>

// Definition
#define TAG "main"
const std::string deviceName = "M5Capsule";

// Global Variable
extern QueueHandle_t queue;

#endif