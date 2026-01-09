#ifndef MQTT_H
#define MQTT_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <Timemark.h>
#include <WiFi.h>

#include "debug.h"
#include "power.h"
#include "secrets.h"

// other modules
extern WiFiClient wifiClient;

// this module
extern PubSubClient mqttClient;
extern Timemark mqttTimeout;
extern Logger logger;

void initMQTT();
void reconnectMQTT();
void MQTTcallback(char* topic, byte* payload, unsigned int length);

#endif  // MQTT_H
