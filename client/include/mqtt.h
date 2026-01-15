#ifndef MQTT_H
#define MQTT_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <Timemark.h>
#include <WiFi.h>

#include <vector>

#include "logger.h"
#include "power.h"
#include "secrets.h"

class MQTTClientWrapper {
 private:
  PubSubClient& client;
  Logger& logger;
  Timemark mqttTimeout;

 public:
  MQTTClientWrapper(PubSubClient& client, Logger& logger);
  void init(std::function<void(char*, byte*, unsigned int)> callbackFunc);
  void loop();
  void reconnect();
  bool subscribeToTopic(const String& topic);

 private:
  std::function<void(char*, byte*, unsigned int)> callbackFunc;
  void callbackWrapper(char* topic, byte* payload, unsigned int length);
  std::vector<String> subscribedTopics;
};

#endif  // MQTT_H
