#include "appliance.h"

void Appliance::updateFromJson(JsonObjectConst json) {
  this->maxPower = json["power"] | 0.0f;
  this->nameNominative = json["name"]["nominative"].as<String>();
  this->nameAccusative = json["name"]["accusative"].as<String>();

  if (json.containsKey("individual_power_meter")) {
    this->hasIndividualPowerMeter = true;
    JsonObjectConst meter = json["individual_power_meter"];
    this->jsonTopicName = meter["json_topic"].as<String>();
    this->jsonFieldName = meter["json_field"].as<String>();
    this->detectionThreshold = meter["detection_threshold"] | 0.0f;
  } else {
    this->hasIndividualPowerMeter = false;
  }
}

bool Appliance::isOn() {
  if (!hasIndividualPowerMeter) {
    return false;
  }

  float currentPower = powerReading.getLast();
  bool ret = currentPower >= detectionThreshold;

  if (ret && !wasOn) {
    logger.debug("Appliance '%s' turned ON, power=%.2fW (threshold=%.2fW), applying max=%.2fW", nameNominative.c_str(), currentPower, detectionThreshold, maxPower);
  } else if (!ret && wasOn) {
    logger.debug("Appliance '%s' turned OFF, power=%.2fW (threshold=%.2fW)", nameNominative.c_str(), currentPower, detectionThreshold);
  }

  wasOn = ret;
  return ret;
}
