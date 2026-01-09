#ifndef BACKLIGHT_H
#define BACKLIGHT_H

#include <Arduino.h>
#include <Timemark.h>

#include "homeassistant.h"

// this module
extern int backlight_on;
extern Timemark backlightTimeout;

void setBacklight(int on_off);
void toggleBacklightManually();
void initBacklight();

#endif  // BACKLIGHT_H
