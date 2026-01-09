#ifndef SYSTEM_H
#define SYSTEM_H

#include <Arduino.h>

void wdtInit();
void wdtRefresh();
void wdtStop();
String resetReasonAsString();
String wakeupReasonAsString();
void logResetReason();

#endif  // SYSTEM_H
