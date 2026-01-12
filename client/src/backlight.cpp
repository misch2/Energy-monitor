#include "backlight.h"

#define SECONDS_TO_MILLIS 1000

Backlight::Backlight() : backlight_on(0), backlightTimeout(30 * SECONDS_TO_MILLIS) {}

void Backlight::setBacklight(int on_off) {  // 0 - 255
  if (backlight_on == on_off) {
    return;
  }

  if (afterChangeCallback) {
    afterChangeCallback(on_off);
  }

  // logger.debug("Setting backlight to %d", on_off);
  if (on_off == 0) {
    ledcWrite(ledc_channel, 0);
  } else {
    ledcWrite(ledc_channel, 255);
  }
  backlight_on = on_off;
}

void Backlight::toggleBacklightManually() {
  // logger.debug("Setting backlight manually to %d", !backlight_on);
  setBacklight(!backlight_on);
  backlightTimeout.stop();
}

void Backlight::init() {
  // PWM backlight on PIN 27
  pinMode(ledc_pin, OUTPUT);
  ledcAttachPin(ledc_pin, ledc_channel);
  ledcSetup(ledc_channel, 5000, 8);
  setBacklight(1);
  backlightTimeout.start();  // turn off backlight after 30 seconds
}

void Backlight::loop() {
  if (backlightTimeout.expired()) {
    setBacklight(0);
    backlightTimeout.stop();
  }
}