#ifndef HOMEASSISTANT_H
#define HOMEASSISTANT_H

#include <Arduino.h>

#include "mqtt.h"

class HomeAssistant {
 private:
  PubSubClient& mqttClient;
  int last_warningstate;
  long last_uptime;

 public:
  HomeAssistant(PubSubClient& mqttClient);

  void publish_value(bool startup, String component, String device_topic, String key, String value, String entity_category, String device_class,
                     String state_class, String unit_of_measurement, String icon);
  void publish_backlight(bool startup, int on_off);
  void publish_warningstate(bool startup, int on_off);
  void publish_uptime(bool startup);
};

#endif  // HOMEASSISTANT_H
