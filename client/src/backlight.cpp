#include "backlight.h"

#define SECONDS_TO_MILLIS 1000

Backlight::Backlight(Logger& logger) : logger(logger), backlightState(false), backlightTimeout(30 * SECONDS_TO_MILLIS) {}

void Backlight::setBacklight(bool on_off) {
  // logger.debug("Backlight::setBacklight called with %d", on_off);
  if (backlightState == on_off) {
    // logger.debug("Backlight state unchanged (%d)", on_off);
    // no change needed
    return;
  }

  if (afterChangeCallback) {
    afterChangeCallback(on_off);
  }

  // logger.debug("Setting backlight to %d", on_off);
  if (on_off == false) {
    ledcWrite(ledc_channel, 0);
  } else {
    ledcWrite(ledc_channel, 255);
  }
  backlightState = on_off;
}

void Backlight::toggleBacklightManually() {
  // logger.debug("Setting backlight manually to %d", !backlightState);
  setBacklight(!backlightState);
  backlightTimeout.stop();
}

void Backlight::init() {
  // PWM backlight on PIN 27
  pinMode(ledc_pin, OUTPUT);
  ledcAttachPin(ledc_pin, ledc_channel);
  ledcSetup(ledc_channel, 5000, 8);
  // logger.debug("Backlight initialized on pin %d, channel %d", ledc_pin, ledc_channel);

  backlightState = false;
  setBacklight(true);

  backlightTimeout.start();  // turn off backlight after 30 seconds
}

void Backlight::loop() {
  if (backlightTimeout.expired()) {
    setBacklight(false);
    backlightTimeout.stop();
  }
}