#ifndef LED_H
#define LED_H

#include <Arduino.h>

class LED {
 public:
  LED();

  void turnOn(int pin);
  void turnOff(int pin);
  void turnOffAll();

  // Pin definitions
  static const int RED_PIN = 4;
  static const int GREEN_PIN = 16;
  static const int BLUE_PIN = 17;

 private:
  void setPin(int pin, bool state);
};

#endif  // LED_H
