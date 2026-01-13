#ifndef BACKLIGHT_H
#define BACKLIGHT_H

#include <Arduino.h>
#include <Timemark.h>

#include "logger.h"

class Backlight {
 private:
  Logger& logger;
  bool backlightState;
  Timemark backlightTimeout;
  std::function<void(bool)> afterChangeCallback;

 public:
  Backlight(Logger& logger);
  void setBacklight(bool on_off);
  void toggleBacklightManually();
  void init();
  void loop();
  void registerAfterChangeCallback(std::function<void(int)> func) {
    afterChangeCallback = func;
  }

  // Getters for external access if needed
  bool isBacklightOn() const {
    return backlightState;
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

#endif  // BACKLIGHT_H
