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

  float getInstantReading();
  float getMovingMax();
  float getMovingAverage();
  void updateReading(float power);

 private:
  CircularBuffer<float, 50> values;
};

class ElectricityMeterConfig {
 public:
  ElectricityMeterConfig(int current_max, int voltage) : maxCurrent(current_max), nominalVoltage(voltage) {}
  ElectricityMeterConfig() {}

  void setMaxCurrent(int current_max) {
    this->maxCurrent = current_max;
  }
  void setNominalVoltage(int voltage) {
    this->nominalVoltage = voltage;
  }
  float getMaxPower() const {
    return static_cast<float>(maxCurrent * nominalVoltage);
  }

  void updateInstantPower(float power) {
    this->powerReading.updateReading(power);
  }
  float getInstantPower() {
    return this->powerReading.getInstantReading();
  }

 private:
  int maxCurrent = 0;
  int nominalVoltage = 0;
  PowerReading powerReading;
};

#endif  // POWER_H
