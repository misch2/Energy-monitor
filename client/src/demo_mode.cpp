#include "demo_mode.h"

void DemoMode::loop() {
  // in demo mode, periodically update power readings with random values

  if (!active || !timer.expired()) {
    return;
  }

  logger.debug("Demo mode: updating power readings with random values");

  float mainMeterPower = random(0, (int)mainMeter.getMaxAllowedWatts());
  mainMeter.powerReading.update(mainMeterPower);

  float totalAppliancePower = 0;
  for (int i = 0; i < appliances.size(); i++) {
    Appliance& appliance = appliances[i];
    if (appliance.hasIndividualPowerMeter) {
      float power = random(1, (int)appliance.maxPower);

      if (random(0, 10) <= 2) {  // set to zero in 20% of cases
        power = 0;
      }

      totalAppliancePower += power;
      if (totalAppliancePower < mainMeterPower) {
        // only set appliance power if total doesn't exceed main meter power
        appliance.powerReading.update(power);
      } else {
        appliance.powerReading.update(0);
      }
    }
  }
}

void DemoMode::setActive(bool mode) {
  active = mode;
  if (active) {
    timer.start();
    logger.debug("Demo mode started");
  } else {
    timer.stop();
    logger.debug("Demo mode stopped");
  }
}

bool DemoMode::isActive() const {
  return active;
}
