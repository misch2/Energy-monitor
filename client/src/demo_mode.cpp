#include "demo_mode.h"

void DemoMode::loop() {
  // in demo mode, periodically update power readings with random values

  if (!active || !timer.expired()) {
    return;
  }

  logger.debug("Demo mode: updating power readings with random values: main meter max=%.2fW", mainMeter.getMaxAllowedWatts());

  float mainMeterPower = random(0, (int)mainMeter.getMaxAllowedWatts());
  mainMeter.powerReading.update(mainMeterPower);

  float totalAppliancePower = 0;
  float totalApplianceWorstCasePower = 0;
  for (int i = 0; i < appliances.size(); i++) {
    Appliance& appliance = appliances[i];
    if (appliance.hasIndividualPowerMeter) {
      float power = random(1, (int)appliance.maxPower);

      if (random(0, 10) < 3) {  // set to zero in 30% of cases
        power = 0;
      }

      totalAppliancePower += power;
      totalApplianceWorstCasePower += appliance.maxPower;
      if (totalAppliancePower < mainMeterPower && totalApplianceWorstCasePower < mainMeter.getMaxAllowedWatts()) {
        // only set appliance power if total doesn't exceed main meter power
        appliance.powerReading.update(power);
        logger.debug(" - Appliance '%s' demo power=%.2fW (max=%.2fW)", appliance.nameNominative.c_str(), power, appliance.maxPower);
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
