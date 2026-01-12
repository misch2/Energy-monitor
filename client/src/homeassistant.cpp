#include "homeassistant.h"

#include <ArduinoJson.h>
#include <PubSubClient.h>

#include "main.h"
#include "secrets.h"

HomeAssistant::HomeAssistant(PubSubClient& mqttClient) : mqttClient(mqttClient), last_warningstate(-1), last_uptime(-1) {}

void HomeAssistant::publish_value(bool startup,                // true if the device is starting up, false if the value is changing
                                  String component,            // component type e.g. "sensor", "text", "switch", etc.
                                  String device_topic,         // device topic name, e.g. "energy_monitor" or "monitor1"
                                  String key,                  // sensor name, e.g. "backlight_status"
                                  String value,                // initial value
                                  String entity_category,      // "diagnostic", "config", etc.
                                  String device_class,         // see https://www.home-assistant.io/integrations/sensor/#device-class
                                  String state_class,          // usually "measurement"
                                  String unit_of_measurement,  // e.g. "W", "V", "A", "kWh", etc.
                                  String icon                  // "mdi:battery" etc.
) {
  // use "sensor" if component is empty
  if (component == "") {
    component = "sensor";
  }

  String config_topic = "homeassistant/" + component + "/" + device_topic + "/" + key + "/config";
  String state_topic = "energy_monitor/" + device_topic + "/state/" + key;

  if (startup) {
    // create HomeAssistant config JSON
    JsonDocument doc;
    doc["state_topic"] = state_topic;
    doc["device"]["manufacturer"] = "Michal";
    doc["device"]["model"] = "Energy Monitor";
    doc["device"]["hw_version"] = "1.0";
    doc["device"]["sw_version"] = VERSION;

    JsonArray identifiers = doc["device"]["identifiers"].to<JsonArray>();
    identifiers.add(device_topic);
    doc["device"]["name"] = device_topic;
    doc["enabled_by_default"] = true;
    doc["entity_category"] = entity_category;
    doc["device_class"] = device_class;
    doc["state_class"] = state_class;
    doc["unit_of_measurement"] = unit_of_measurement;
    doc["icon"] = icon;
    // 2024-02-09 16:17:16.633 WARNING (MainThread) [homeassistant.components.mqtt.mixins] MQTT entity name starts with the device name in your config
    // {'state_topic': 'energy_monitor/energymonitor1/state/backlight', 'device': {'manufacturer': 'Michal', 'model': 'Energy Monitor', 'identifiers':
    // ['energymonitor1'], 'name': 'energymonitor1', 'connections': []}, 'enabled_by_default': True, 'entity_category': <EntityCategory.DIAGNOSTIC:
    // 'diagnostic'>, 'device_class': <BinarySensorDeviceClass.LIGHT: 'light'>, 'icon': 'mdi:lightbulb', 'name': 'energymonitor1 backlight', 'unique_id':
    // 'energymonitor1_backlight', 'force_update': False, 'qos': 0, 'availability_mode': 'latest', 'payload_on': 'ON', 'payload_not_available': 'offline',
    // 'encoding': 'utf-8', 'payload_off': 'OFF', 'payload_available': 'online'}, this is not expected. Please correct your configuration. The device name
    // prefix will be stripped off the entity name and becomes 'backlight'
    doc["name"] = key;  // fixed, see above^
    doc["unique_id"] = device_topic + "_" + key;

    String serialized;
    serializeJson(doc, serialized);
    mqttClient.publish(config_topic.c_str(), serialized.c_str(), true);  // publish as retained, to survive HA restart
  }
  mqttClient.publish(state_topic.c_str(), value.c_str(), false);
}

void HomeAssistant::publish_backlight(bool startup, int on_off) {
  publish_value(startup, "binary_sensor", MQTT_HA_DEVICENAME, "backlight", on_off ? "ON" : "OFF", "diagnostic",
                "light", /* one of 'battery', 'battery_charging', 'carbon_monoxide', 'cold', 'connectivity', 'door', 'garage_door', 'gas', 'heat',
                            'light', 'lock', 'moisture', 'motion', 'moving', 'occupancy', 'opening', 'plug', 'power', 'presence', 'problem',
                            'running', 'safety', 'smoke', 'sound', 'tamper', 'update', 'vibration', 'window' */
                "measurement", "", "mdi:lightbulb");
}

void HomeAssistant::publish_warningstate(bool startup, int on_off) {
  if (on_off == last_warningstate && !startup) {
    return;
  }
  publish_value(startup, "binary_sensor", MQTT_HA_DEVICENAME, "warning", on_off ? "ON" : "OFF", "diagnostic",
                "power", /* one of 'battery', 'battery_charging', 'carbon_monoxide', 'cold', 'connectivity', 'door', 'garage_door', 'gas', 'heat',
                            'light', 'lock', 'moisture', 'motion', 'moving', 'occupancy', 'opening', 'plug', 'power', 'presence', 'problem',
                            'running', 'safety', 'smoke', 'sound', 'tamper', 'update', 'vibration', 'window' */
                "measurement", "", "mdi:power-socket-eu");
  last_warningstate = on_off;
}

void HomeAssistant::publish_uptime_if_needed(bool startup) {
  long uptime = millis() / 1000;
  if ((uptime - last_uptime >= 30) || (uptime < last_uptime) || startup) {  // publish every minute or if the value overflowed
    publish_value(startup, "sensor", MQTT_HA_DEVICENAME, "uptime", String(uptime), "diagnostic", "duration", "measurement", "s", "mdi:chart-box-outline");
    last_uptime = uptime;
  }
}
