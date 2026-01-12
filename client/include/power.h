#ifndef POWER_H
#define POWER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

#include "circular_buffer.h"
#include "logger.h"

class PowerReading {
 public:
  PowerReading() {}

  float getLast();
  float getMovingMax(size_t numValues = 10);
  float getMovingAverage(size_t numValues = 10);
  void update(float power);

 private:
  SmartCircularBuffer<float, 50> values;
};

class ElectricityMeter {
 public:
  PowerReading powerReading;
  int maxCurrent = 0;
  int nominalVoltage = 0;

  ElectricityMeter(int current_max, int nominalVoltage) : maxCurrent(current_max), nominalVoltage(nominalVoltage) {}
  ElectricityMeter() {}

  void setMaxCurrent(int current_max) {
    this->maxCurrent = current_max;
  }
  void setNominalVoltage(int nominalVoltage) {
    this->nominalVoltage = nominalVoltage;
  }
  float getMaxAllowedWatts() const {
    return static_cast<float>(maxCurrent * nominalVoltage);
  }
};

#endif  // POWER_H
