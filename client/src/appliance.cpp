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
  return currentPower >= detectionThreshold;
}
