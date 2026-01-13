#ifndef APPLIANCE_H
#define APPLIANCE_H

#include <Arduino.h>
#include <ArduinoJson.h>

#include <vector>

#include "power.h"

class Appliance {
 public:
  String nameNominative;
  String nameAccusative;
  float maxPower;

  // Individual power meter (optional)
  bool hasIndividualPowerMeter;
  String jsonTopicName;
  String jsonFieldName;
  float detectionThreshold;
  PowerReading powerReading;

  Appliance() : maxPower(0), hasIndividualPowerMeter(false), detectionThreshold(0) {}

  Appliance(const String& nomName, const String& accName, float pwr)
      : nameNominative(nomName), nameAccusative(accName), maxPower(pwr), hasIndividualPowerMeter(false), detectionThreshold(0) {}

  Appliance(const String& nomName, const String& accName, float pwr, const String& topic, const String& field, float threshold)
      : nameNominative(nomName),
        nameAccusative(accName),
        maxPower(pwr),
        hasIndividualPowerMeter(true),
        jsonTopicName(topic),
        jsonFieldName(field),
        detectionThreshold(threshold) {}

  static Appliance fromJson(JsonObjectConst json);
  bool isOn();
};

using ApplianceList = std::vector<Appliance>;

#endif  // APPLIANCE_H