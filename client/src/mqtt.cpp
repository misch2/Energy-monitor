#include "mqtt.h"

#define SECONDS_TO_MILLIS 1000


Timemark mqttTimeout(60 * SECONDS_TO_MILLIS);


// handle message arrived
void MQTTcallback(char* topic, byte* payload, unsigned int length) {
  String topicString = String(topic);
  String payloadString = "";
  for (int i = 0; i < length; i++) {
    payloadString += (char)payload[i];
  }

  // logger.debug("Message arrived on topic [" + topicString + "] Payload: [" + payloadString + "]");

  if (topicString.equals(mqttTopicCurrentPower)) {
    handleMQTTMessageCurrentPower(payloadString);
  } else if (topicString.equals(MQTT_CONFIGURATION_TOPIC)) {
    handleMQTTMessageConfiguration(payloadString);
  } else {
    // try to find individual power meter topics
    bool found = false;
    JsonDocument individual_meter;
    // logger.debug("Checking %d appliances for individual power meter topics", appliances.size());
    for (int i = 0; i < appliances.size(); i++) {
      JsonVariant appliance = appliances[i];
      // try to get appliance topic as a String
      String appliance_topic = appliance["individual_power_meter"]["json_topic"];
      if (appliance_topic.equals(topicString)) {
        found = true;

        // logger.debug("Handling individual power meter topic [%s]", topicString.c_str());
        String json_field = appliance["individual_power_meter"]["json_field"];
        float detection_threshold = appliance["individual_power_meter"]["detection_threshold"];

        // parse payload as JSON and get the value at json_field (e.g. { "power": 123.4 } and json_field = "power" -> 123.4)
        DeserializationError error = deserializeJson(individual_meter, payloadString);
        if (error) {
          logger.debug("Parsing individual power meter input failed: %s", error.c_str());
          return;
        }
        float individual_power = individual_meter[json_field];
        // logger.debug("Individual power meter topic [%s], path [%s] -> power: %.2f W", topicString.c_str(), json_field.c_str(), individual_power);

        float correction = 0;
        if (individual_power >= detection_threshold) {
          // appliance is ON, update the worst-case power consumption
          correction = (float)appliance["power"] - individual_power;  // 900 W (max) - 35 W (measured) = 865 W (correction)
          // logger.debug("Appliance #%d is ON, individual power %.2f W >= detection threshold %.2f W, setting worst-case power consumption correction to %.2f
          // W",
          //             i + 1, individual_power, detection_threshold, correction);
          // logger.debug("  (appliance max power %.2f W - individual power %.2f W = correction %.2f W)", (float)appliance["power"], individual_power,
          // correction);
        }
        applianceWorstCaseCorrection[i] = correction;
      }
    }
    if (!found) {
      logger.debug("Unknown topic [%s]", topicString.c_str());
    }
  }

  mqttTimeout.start();  // reset the timeout
}

void initMQTT() {
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(MQTTcallback);
}

void reconnectMQTT() {
  if (mqttClient.connected()) {
    return;
  }

  while (!mqttClient.connected()) {
    logger.debug("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect("EnergyMonitor/1.0", MQTT_USER, MQTT_PASSWORD)) {
      logger.debug(" - connected");
      logger.debug("Subscribing to configuration topic [%s]", MQTT_CONFIGURATION_TOPIC);
      bool ok = mqttClient.subscribe(MQTT_CONFIGURATION_TOPIC);
      // logger.debug(ok ? "Subscribed" : "Subscription failed");
    } else {
      logger.debug(" - failed, rc=%d", mqttClient.state());
      logger.debug(" - trying again in 5 seconds");
      delay(5 * SECONDS_TO_MILLIS);
    }
  }

  mqttTimeout.start();
}
