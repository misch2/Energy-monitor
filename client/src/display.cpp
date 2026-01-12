#include "display.h"

#define SECONDS_TO_MILLIS 1000
#define MILLIS 1

// landscape, USB on the right
// #define ROTATE_TFT 1
// #define ROTATE_TOUCH ROTATION_RIGHT

// portrait, USB at the bottom
#define ROTATE_TFT 0
#define ROTATE_TOUCH ROTATION_INVERTED  // NORMAL, LEFT, INVERTED, RIGHT

// static methods
void Display::_disp_flush_callback(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p) {
  auto* instance = static_cast<Display*>(disp_drv->user_data);
  if (instance) {
    instance->my_disp_flush(disp_drv, area, color_p);
  }
}
void Display::_touchpad_read_callback(lv_indev_drv_t* indev_driver, lv_indev_data_t* data) {
  auto* instance = static_cast<Display*>(indev_driver->user_data);
  if (instance) {
    instance->my_touchpad_read(indev_driver, data);
  }
}

/* Serial debugging */
void Display::my_print(const char* buf) {
  // LV_UNUSED(level);
  logger.debug(buf);
}

/* Display flushing */
void Display::my_disp_flush(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  // logger.debug("Flushing area: x1=%d, y1=%d, x2=%d, y2=%d, w=%d, h=%d", area->x1, area->y1, area->x2, area->y2, w, h);
  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t*)&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp_drv);
}

void Display::my_touchpad_read(lv_indev_drv_t* indev_driver, lv_indev_data_t* data) {
  tp.read();
  bool touched = tp.isTouched && tp.touches == 1;  // allowing only single touch here
  if (!touched) {
    data->state = LV_INDEV_STATE_REL;
  } else {
    data->state = LV_INDEV_STATE_PR;

    data->point.x = tp.points[0].x;
    data->point.y = tp.points[0].y;
  }
}

void Display::init() {
  leds.turnOffAll();
  backlight.setBacklight(1);

  tp.begin();
  tp.setRotation(ROTATE_TOUCH);

  initLVGL();
}

void Display::initLVGL() {
  lv_init();

#if LV_USE_LOG != 0
  lv_log_register_print_cb([this](const char* buf) { my_print(buf); }); /* register print function for debugging */
#endif

  tft.begin(); /* TFT init */
  tft.setRotation(ROTATE_TFT);

  /* Buffer to draw to */
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, drawBufferSize);

  /* Driver definition */
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = Display::_disp_flush_callback;  // static method
  disp_drv.draw_buf = &draw_buf;
  disp_drv.user_data = this;  // attach the instance pointer for use in callbacks
  lv_disp_drv_register(&disp_drv);

  /*Initialize the (dummy) input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = Display::_touchpad_read_callback;  // static method
  indev_drv.user_data = this;                            // attach the instance pointer for use in callbacks
  lv_indev_drv_register(&indev_drv);
}

void Display::refresh_lv_tick_value() {
  // LV_TICK_CUSTOM in lvgl.h doesn't work for some reason, neither as Arduino nor as ESP-IDF.
  // Therefore this hack:
  // lv_tick_inc(max(LV_DISP_DEF_REFR_PERIOD, LV_INDEV_DEF_READ_PERIOD));
  unsigned long now = millis();
  if (now < last_millis) {  // overflow
    last_millis = now;
    lv_tick_inc(1 * SECONDS_TO_MILLIS);
  } else {  // increment
    lv_tick_inc(now - last_millis);
    last_millis = now;
  }
}

void Display::loop() {
  refresh_lv_tick_value();
  lv_timer_handler();
  systemLayer.wdtRefresh();
}

void Display::setLoadingScreenText(const char* text) {
  logger.debug(text);
  lv_label_set_text(ui_LoadingLabel, text);
  loop();
}

bool Display::showApplianceLabel(lv_obj_t* ui_element, int number, int remainingWatts) {
  // logger.debug("in showApplianceLabel(%d, %d)", number, remainingWatts);

  if (number > appliances.size()) {
    // logger.debug("Appliance #%d not found in config, skipping this label", number);
    lv_obj_add_flag(ui_element, LV_OBJ_FLAG_HIDDEN);
    return false;
  }

  Appliance appliance = appliances[number - 1];
  if (remainingWatts > appliance.maxPower) {
    // logger.debug("Appliance %d compliant, remainingWatts %d > %d, skipping this label", number, remainingWatts, (int)appliance["power"]);
    lv_obj_add_flag(ui_element, LV_OBJ_FLAG_HIDDEN);
    return false;
  };

  if (appliance.isOn()) {
    // display it too, but in a dimmed color
    lv_obj_set_style_bg_color(ui_element, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
  } else {
    // normal bright warning color
    lv_obj_set_style_bg_color(ui_element, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  }

  String name = appliance.nameAccusative;
  // logger.debug("Appliance %d displayed, remainingWatts %d <= %d, name: %s", number, remainingWatts, (int)appliance["power"], name.c_str());
  lv_label_set_text(ui_element, name.c_str());
  lv_obj_clear_flag(ui_element, LV_OBJ_FLAG_HIDDEN);

  return true;
}

void Display::handleElectricityMeterConfigChange(float maxPowerWatts) {
  lv_arc_set_range(ui_ArcCurrentWattsOK, 0, maxPowerWatts);
  lv_arc_set_range(ui_ArcCurrentWattsWarning, 0, maxPowerWatts);
  lv_arc_set_range(ui_ArcWorstCaseWattsOK, 0, maxPowerWatts);
  lv_arc_set_range(ui_ArcWorstCaseWattsWarning, 0, maxPowerWatts);
}

bool Display::updateFromPowerReading(ElectricityMeter meter) {
  float currentWatts = meter.powerReading.getMovingAverage(3);
  float maxUsedWatts = meter.powerReading.getMovingMax(5);

  float remainingWatts = meter.getMaxAllowedWatts() - maxUsedWatts;  // time filtered and rounded down value for the warning message, to prevent flickering
  int displayedRemainingWatts = (int)(remainingWatts / 50) * 50;     // round down to multiples of 50

  // non-filtered and more precise value for the gauge
  lv_arc_set_value(ui_ArcCurrentWattsOK, currentWatts);
  lv_arc_set_value(ui_ArcCurrentWattsWarning, currentWatts);

  // Calculate worst-case power consumption based on the individual appliances
  float currentWorstCaseWatts = currentWatts;
  for (auto appliance : appliances) {
    if (appliance.isOn()) {
      currentWorstCaseWatts += appliance.maxPower - appliance.powerReading.getMovingMax(3);
    }
  }

  lv_arc_set_value(ui_ArcWorstCaseWattsOK, currentWorstCaseWatts);
  lv_arc_set_value(ui_ArcWorstCaseWattsWarning, currentWorstCaseWatts);

  String label = "";
  label += displayedRemainingWatts;
  label += " W";
  lv_label_set_text(ui_LabelRemainingWattsOK, label.c_str());
  lv_label_set_text(ui_LabelRemainingWattsWarning, label.c_str());

  label = "";
  label += (int)(currentWatts);
  label += " W";
  lv_label_set_text(ui_LabelWattsUsedOK, label.c_str());
  lv_label_set_text(ui_LabelWattsUsedWarning, label.c_str());

  bool displayed_warning = false;
  displayed_warning |= showApplianceLabel(ui_LabelAppliance1, 1, displayedRemainingWatts);
  displayed_warning |= showApplianceLabel(ui_LabelAppliance2, 2, displayedRemainingWatts);
  displayed_warning |= showApplianceLabel(ui_LabelAppliance3, 3, displayedRemainingWatts);
  displayed_warning |= showApplianceLabel(ui_LabelAppliance4, 4, displayedRemainingWatts);
  displayed_warning |= showApplianceLabel(ui_LabelAppliance5, 5, displayedRemainingWatts);
  displayed_warning |= showApplianceLabel(ui_LabelAppliance6, 6, displayedRemainingWatts);
  displayed_warning |= showApplianceLabel(ui_LabelAppliance7, 7, displayedRemainingWatts);
  displayed_warning |= showApplianceLabel(ui_LabelAppliance8, 8, displayedRemainingWatts);
  displayed_warning |= showApplianceLabel(ui_LabelAppliance9, 9, displayedRemainingWatts);
  displayed_warning |= showApplianceLabel(ui_LabelAppliance10, 10, displayedRemainingWatts);
  displayed_warning |= showApplianceLabel(ui_LabelAppliance11, 11, displayedRemainingWatts);
  displayed_warning |= showApplianceLabel(ui_LabelAppliance12, 12, displayedRemainingWatts);

  if (displayed_warning) {
    lv_disp_load_scr(ui_WarningScreen);
    backlight.setBacklight(1);
    backlight.startTimeout();
    return 0;
  };

  lv_disp_load_scr(ui_OKScreen);
  return 1;
}
