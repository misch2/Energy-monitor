#include "power.h"

float PowerReading::getInstantReading() {
  return this->values.getLast();
}

float PowerReading::getMovingMax(size_t numValues) {
  return this->values.maxValue(numValues);
}

float PowerReading::getMovingAverage(size_t numValues) {
  return this->values.averageValue(numValues);
}

void PowerReading::updateReading(float power) {
  this->values.push(power);
}
