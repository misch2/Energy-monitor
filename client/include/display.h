#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <TAMC_GT911.h>
#include <TFT_eSPI.h>
#include <lvgl.h>

#include "appliance.h"
#include "backlight.h"
#include "led.h"
#include "logger.h"
#include "power.h"
#include "system.h"
#include "ui/ui.h"

class Display {
 private:
  LED& leds;
  Backlight& backlight;
  Logger& logger;
  SystemLayer& systemLayer;

  /* LOGICAL screen orientation (i.e. rotation dependent) */
  static const uint16_t screenWidth = 320;
  static const uint16_t screenHeight = 480;
  static const uint16_t drawBufferSize = (screenWidth * screenHeight / 10);

  lv_disp_draw_buf_t draw_buf;
  lv_color_t buf[drawBufferSize];

  unsigned long last_millis = millis();

  TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight);                                                 // display
  TAMC_GT911 tp = TAMC_GT911(TOUCH_SDA, TOUCH_SCL, TOUCH_INT, TOUCH_RST, screenWidth, screenHeight);  // touch panel

 public:
  Display(LED& leds, Backlight& backlight, Logger& logger, SystemLayer& systemLayer)
      : leds(leds), backlight(backlight), logger(logger), systemLayer(systemLayer) {}
  void init();
  void loop();
  void setLoadingScreenText(const char* text);
  bool showApplianceLabel(lv_obj_t* ui_element, ApplianceList& appliances, int number, float remainingWatts);
  void handleElectricityMeterConfigChange(float maxPowerWatts);
  bool updateFromPowerReading(ApplianceList& appliances, ElectricityMeter& meter);

 private:
  // static methods to be used as lvgl callbacks
  static void _disp_flush_callback(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p);
  static void _touchpad_read_callback(lv_indev_drv_t* indev_driver, lv_indev_data_t* data);

  void initLVGL();
  void my_print(const char* buf);
  void my_disp_flush(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p);
  void my_touchpad_read(lv_indev_drv_t* indev_driver, lv_indev_data_t* data);
  void refresh_lv_tick_value();
};

#endif  // DISPLAY_H
