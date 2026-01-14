#ifndef APPLIANCE_H
#define APPLIANCE_H

#include <Arduino.h>
#include <ArduinoJson.h>

#include <vector>

#include "logger.h"
#include "power.h"

class Appliance {
 public:
  Logger& logger;
  String nameNominative;
  String nameAccusative;
  float maxPower;

  // Individual power meter (optional)
  bool hasIndividualPowerMeter;
  String jsonTopicName;
  String jsonFieldName;
  float detectionThreshold;
  PowerReading powerReading;

  Appliance(Logger& logger) : logger(logger), maxPower(0), hasIndividualPowerMeter(false), detectionThreshold(0) {}
  void updateFromJson(JsonObjectConst json);
  bool isOn();
};

using ApplianceList = std::vector<Appliance>;

#endif  // APPLIANCE_H