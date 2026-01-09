#ifndef HOMEASSISTANT_H
#define HOMEASSISTANT_H

#include <Arduino.h>

#include "mqtt.h"

// other modules
extern PubSubClient mqttClient;

void publish_homeassistant_value(bool startup, String component, String device_topic, String key, String value, String entity_category, String device_class,
                                 String state_class, String unit_of_measurement, String icon);
void publish_homeassistant_value_backlight(bool startup, int on_off);
void publish_homeassistant_value_warningstate(bool startup, int on_off);
void publish_homeassistant_value_uptime(bool startup);

#endif  // HOMEASSISTANT_H
