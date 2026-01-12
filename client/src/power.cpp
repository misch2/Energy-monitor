#include "power.h"

float PowerReading::getInstantReading() {
  return this->instantPower;
}

float PowerReading::getMovingMaxReading() {
  return this->movingMaxPower;
}

void PowerReading::updateReading(float power) {
  this->instantPower = power;

  // add currentWatts to buffer and find max over last MAX_POWER_READINGS_BUFFER readings
  maxReadingsBuffer.push(power);

  movingMaxPower = instantPower;
  for (size_t i = 0; i < maxReadingsBuffer.size(); i++) {
    float reading = maxReadingsBuffer.get(i);
    if (reading > movingMaxPower) {
      movingMaxPower = reading;
    }
  }
}
