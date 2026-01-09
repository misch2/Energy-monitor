#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <TAMC_GT911.h>
#include <TFT_eSPI.h>
#include <lvgl.h>

#include "backlight.h"
#include "debug.h"
#include "led.h"
#include "system.h"
#include "ui/ui.h"

// this module
extern TFT_eSPI tft;
extern TAMC_GT911 tp;

void initDisplay();
void initLVGL();
void refresh_lv_tick_value();
void refresh_screen();
void setLoadingScreenText(const char* text);

#endif  // DISPLAY_H
