#pragma once

#include <Arduino.h>
#include <Timemark.h>

#include "appliance.h"
#include "logger.h"
#include "power.h"

#define SECONDS_TO_MILLIS 1000

class DemoMode {
 private:
  Logger& logger;
  ApplianceList& appliances;
  ElectricityMeter& mainMeter;

 public:
  DemoMode(Logger& logger, ApplianceList& appliances, ElectricityMeter& mainMeter) : logger(logger), appliances(appliances), mainMeter(mainMeter) {}
  void loop();
  bool isActive() const;
  void setActive(bool mode);

 private:
  bool active = false;
  Timemark timer = Timemark(10 * SECONDS_TO_MILLIS);  // update every 10 seconds
};