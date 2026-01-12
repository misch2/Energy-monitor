#ifndef APPLIANCE_H
#define APPLIANCE_H

#include <Arduino.h>
#include <ArduinoJson.h>

#include <vector>

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

  static Appliance fromJson(JsonObjectConst json) {
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
};

#endif  // APPLIANCE_H