#ifndef SYSTEM_H
#define SYSTEM_H

#include <Arduino.h>

#include "logger.h"

class SystemLayer {
 private:
  Logger& logger;

 public:
  SystemLayer(Logger& logger) : logger(logger) {}

  void wdtInit();
  void wdtRefresh();
  void wdtStop();
  String resetReasonAsString();
  String wakeupReasonAsString();
  void logResetReason();
  bool resetReasonWasFaultySoftware();
};

#endif  // SYSTEM_H
