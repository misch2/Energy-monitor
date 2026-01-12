#include "power.h"

float PowerReading::getInstantReading() {
  return this->values.getLast();
}

float PowerReading::getMovingMax() {
  return this->values.maxValue();
}

float PowerReading::getMovingAverage() {
  return this->values.averageValue();
}

void PowerReading::updateReading(float power) {
  this->values.push(power);
}
