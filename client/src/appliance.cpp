#include "appliance.h"

Appliance Appliance::fromJson(JsonObjectConst json) {
  Appliance app;
  app.maxPower = json["power"] | 0.0f;
  app.nameNominative = json["name"]["nominative"].as<String>();
  app.nameAccusative = json["name"]["accusative"].as<String>();

  if (json.containsKey("individual_power_meter")) {
    app.hasIndividualPowerMeter = true;
    JsonObjectConst meter = json["individual_power_meter"];
    app.jsonTopicName = meter["json_topic"].as<String>();
    app.jsonFieldName = meter["json_field"].as<String>();
    app.detectionThreshold = meter["detection_threshold"] | 0.0f;
  }

  return app;
}

bool Appliance::isOn() {
  if (!hasIndividualPowerMeter) {
    return false;
  }
  float currentPower = powerReading.getInstantReading();
  return currentPower >= detectionThreshold;
}
