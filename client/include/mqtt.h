#ifndef MQTT_H
#define MQTT_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <Timemark.h>
#include <WiFi.h>

#include "logger.h"
#include "power.h"
#include "secrets.h"

class MQTTClientWrapper {
 private:
  PubSubClient& client;
  Logger& logger;

  Timemark mqttTimeout;
  std::function<void(char*, byte*, unsigned int)> callbackFunc;

 public:
  MQTTClientWrapper(PubSubClient& client, Logger& logger);
  void init(std::function<void(char*, byte*, unsigned int)> callbackFunc);
  void reconnect();
  void loop();

 private:
  void callbackWrapper(char* topic, byte* payload, unsigned int length);
};

#endif  // MQTT_H
