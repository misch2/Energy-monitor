#ifndef POWER_H
#define POWER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

#include <CircularBuffer.hpp>
#include <array>

#include "backlight.h"
#include "debug.h"
#include "display.h"
#include "homeassistant.h"
#include "ui/ui.h"

#define MAX_APPLIANCES 12  // as in the UI files (number of the last ui_LabelApplianceXX)

// other modules
extern PubSubClient mqttClient;
extern Logger logger;

// this module
extern JsonDocument config;
extern String mqttTopicCurrentPower;
extern int voltage;
extern int maxCurrent;
extern int limitWatts;
extern JsonArray appliances;
extern std::array<float, MAX_APPLIANCES> applianceWorstCaseCorrection;

extern float currentRealWatts;
extern float filteredCurrentRealWatts;
extern float currentWorstCaseWatts;
extern float filteredCurrentWorstCaseWatts;

void handleMQTTMessageCurrentPower(String payloadString);
void handleMQTTMessageConfiguration(String payloadString);
void updateCurrentPower();
bool showApplianceLabel(lv_obj_t* ui_element, int number, int remainingWatts);

#endif  // POWER_H
