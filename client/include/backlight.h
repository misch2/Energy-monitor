#ifndef BACKLIGHT_H
#define BACKLIGHT_H

#include <Arduino.h>
#include <Timemark.h>

#include "homeassistant.h"

class HomeAssistant;  // forward declaration

class Backlight {
 private:
  HomeAssistant& homeassistant;
  int backlight_on;
  Timemark backlightTimeout;

 public:
  Backlight(HomeAssistant& ha);

  void setBacklight(int on_off);
  void toggleBacklightManually();
  void init();
  void loop();

  // Getters for external access if needed
  int isBacklightOn() const {
    return backlight_on;
  }
  Timemark& getTimeout() {
    return backlightTimeout;
  }
  void startTimeout() {
    backlightTimeout.start();
  }
  void stopTimeout() {
    backlightTimeout.stop();
  }

  static const int ledc_pin = 27;
  static const int ledc_channel = 0;
};

extern Backlight backlight;

#endif  // BACKLIGHT_H
