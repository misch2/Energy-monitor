#include "mqtt.h"

#define SECONDS_TO_MILLIS 1000

MQTTClientWrapper::MQTTClientWrapper(PubSubClient& client, Logger& logger) : client(client), logger(logger), mqttTimeout(60 * SECONDS_TO_MILLIS) {}

void MQTTClientWrapper::init(std::function<void(char*, byte*, unsigned int)> callbackFunc) {
  client.setServer(MQTT_SERVER, MQTT_PORT);

  this->callbackFunc = callbackFunc;
  client.setCallback([this](char* topic, byte* payload, unsigned int length) { this->callbackWrapper(topic, payload, length); });

  reconnect();
}

void MQTTClientWrapper::reconnect() {
  if (client.connected()) {
    return;
  }

  while (!client.connected()) {
    logger.debug("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("EnergyMonitor/1.0", MQTT_USER, MQTT_PASSWORD)) {
      logger.debug(" - connected");
      logger.debug("Subscribing to configuration topic [%s]", MQTT_CONFIGURATION_TOPIC);
      bool ok = client.subscribe(MQTT_CONFIGURATION_TOPIC);
      // logger.debug(ok ? "Subscribed" : "Subscription failed");
    } else {
      logger.debug(" - failed, rc=%d", client.state());
      logger.debug(" - trying again in 5 seconds");
      delay(5 * SECONDS_TO_MILLIS);
    }
  }

  mqttTimeout.start();
}

void MQTTClientWrapper::loop() {
  if (mqttTimeout.expired()) {
    // logger.debug("MQTT timeout, reconnecting");
    // mqttClient.disconnect();
    // mqttTimeout.stop();
    logger.debug("No MQTT message arrived for %d s, connected=%d, state=%d", mqttTimeout.limitMillis() / SECONDS_TO_MILLIS, client.connected(), client.state());

    if (WiFi.status() == WL_CONNECTED && !client.connected()) {
      logger.debug("MQTT not connected, reconnecting");
      reconnect();
    } else {
      logger.debug("Fully rebooting after 60 seconds", mqttTimeout.limitMillis() / SECONDS_TO_MILLIS);
      delay(60 * SECONDS_TO_MILLIS);
      logger.debug("rebooting now");
      ESP.restart();
    }
  }
}

void MQTTClientWrapper::callbackWrapper(char* topic, byte* payload, unsigned int length) {
  if (callbackFunc) {
    callbackFunc(topic, payload, length);
  }
  mqttTimeout.start();  // reset the timeout
}