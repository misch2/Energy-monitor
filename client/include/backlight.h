#ifndef BACKLIGHT_H
#define BACKLIGHT_H

#include <Arduino.h>
#include <Timemark.h>

class Backlight {
 private:
  int backlight_on;
  Timemark backlightTimeout;
  std::function<void(int)> afterChangeCallback;

 public:
  Backlight();

  void setBacklight(int on_off);
  void toggleBacklightManually();
  void init();
  void loop();
  void registerAfterChangeCallback(std::function<void(int)> func) {
    afterChangeCallback = func;
  }

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

#endif  // BACKLIGHT_H
