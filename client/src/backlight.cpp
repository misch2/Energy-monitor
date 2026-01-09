#include "backlight.h"

#define BL_LEDC_PIN 27
#define BL_LEDC_CHANNEL 0
#define SECONDS_TO_MILLIS 1000

int backlight_on = 0;
Timemark backlightTimeout(30 * SECONDS_TO_MILLIS);

void setBacklight(int on_off) {  // 0 - 255
  if (backlight_on == on_off) {
    return;
  }

  publish_homeassistant_value_backlight(false, on_off);

  // logger.debug("Setting backlight to %d", on_off);
  if (on_off == 0) {
    ledcWrite(BL_LEDC_CHANNEL, 0);
  } else {
    ledcWrite(BL_LEDC_CHANNEL, 255);
  }
  backlight_on = on_off;
}

void toggleBacklightManually() {
  // logger.debug("Setting backlight manually to %d", !backlight_on);
  setBacklight(!backlight_on);
  backlightTimeout.stop();
}

void initBacklight() {
  // PWM backlight on PIN 27
  pinMode(BL_LEDC_PIN, OUTPUT);
  ledcAttachPin(BL_LEDC_PIN, BL_LEDC_CHANNEL);
  ledcSetup(BL_LEDC_CHANNEL, 5000, 8);
  setBacklight(1);
  backlightTimeout.start();  // turn off backlight after 30 seconds
}
