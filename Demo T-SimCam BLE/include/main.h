#ifndef __MAIN_H__
#define __MAIN_H__

// Include Necessary Header
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Global Variables
extern QueueHandle_t sensorQ;

#endif