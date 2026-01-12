#ifndef POWER_H
#define POWER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

#include <array>

#include "logger.h"

template <typename T, size_t N>
class CircularBuffer {
 public:
  CircularBuffer() : head(0), count(0) {}

  void push(T value) {
    buffer[head] = value;
    head = (head + 1) % N;
    if (count < N) {
      count++;
    }
  }

  T get(size_t index) const {
    if (index >= count) {
      return T();
    }
    size_t actualIndex = (head + N - count + index) % N;
    return buffer[actualIndex];
  }

  size_t size() const {
    return count;
  }

  bool isFull() const {
    return count == N;
  }

  void clear() {
    head = 0;
    count = 0;
  }

 private:
  std::array<T, N> buffer;
  size_t head;
  size_t count;
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
    this->instantPower = power;
  }
  float getInstantPower() const {
    return this->instantPower;
  }

 private:
  int maxCurrent = 0;
  int nominalVoltage = 0;
  float instantPower = 0;
};

class PowerReading {
 public:
  PowerReading() : instantPower(0), movingMaxPower(0) {}

  float getInstantReading();
  float getMovingMaxReading();
  void updateReading(float power);

 private:
  float instantPower;
  static const int MAX_POWER_READINGS_BUFFER = 10;
  CircularBuffer<float, MAX_POWER_READINGS_BUFFER> maxReadingsBuffer;
  float movingMaxPower;
};

#endif  // POWER_H
