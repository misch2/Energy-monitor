#include "led.h"

LED::LED() {
  // Constructor - initialize if needed
}

void LED::setPin(int pin, bool state) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, state ? LOW : HIGH);  // Active LOW logic
}

void LED::turnOn(int pin) {
  setPin(pin, true);
}

void LED::turnOff(int pin) {
  setPin(pin, false);
}

void LED::turnOffAll() {
  turnOff(RED_PIN);
  turnOff(GREEN_PIN);
  turnOff(BLUE_PIN);
}


