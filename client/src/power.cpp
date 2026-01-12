#include "power.h"

float PowerReading::getLast() {
  return this->values.getLast();
}

float PowerReading::getMovingMax(size_t numValues) {
  return this->values.maxValue(numValues);
}

float PowerReading::getMovingAverage(size_t numValues) {
  return this->values.averageValue(numValues);
}

void PowerReading::update(float power) {
  this->values.push(power);
}
