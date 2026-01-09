#include "display.h"

#define SECONDS_TO_MILLIS 1000
#define MILLIS 1

// landscape, USB on the right
// #define ROTATE_TFT 1
// #define ROTATE_TOUCH ROTATION_RIGHT

// portrait, USB at the bottom
#define ROTATE_TFT 0
#define ROTATE_TOUCH ROTATION_INVERTED  // NORMAL, LEFT, INVERTED, RIGHT

/* LOGICAL screen orientation (i.e. rotation dependent) */
static const uint16_t screenWidth = 320;
static const uint16_t screenHeight = 480;

static lv_disp_draw_buf_t draw_buf;
#define DRAW_BUF_SIZE (screenWidth * screenHeight / 10)
static lv_color_t buf[DRAW_BUF_SIZE];

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */
TAMC_GT911 tp = TAMC_GT911(TOUCH_SDA, TOUCH_SCL, TOUCH_INT, TOUCH_RST, screenWidth, screenHeight);

#if LV_USE_LOG != 0
/* Serial debugging */
// void my_print( lv_log_level_t level, const char * buf )
void my_print(const char* buf) {
  // LV_UNUSED(level);
  logger.debug(buf);
}
#endif

/* Display flushing */
void my_disp_flush(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  // logger.debug("Flushing area: x1=%d, y1=%d, x2=%d, y2=%d, w=%d, h=%d", area->x1, area->y1, area->x2, area->y2, w, h);
  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t*)&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp_drv);
}

void my_touchpad_read(lv_indev_drv_t* indev_driver, lv_indev_data_t* data) {
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

void initDisplay() {
  leds.turnOffAll();
  backlight.init();

  tp.begin();
  tp.setRotation(ROTATE_TOUCH);
}

void initLVGL() {
  lv_init();

#if LV_USE_LOG != 0
  lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

  tft.begin(); /* TFT init */
  tft.setRotation(ROTATE_TFT);

  /* Buffer to draw to */
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, DRAW_BUF_SIZE);

  /* Driver definition */
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the (dummy) input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);
}

unsigned long last_millis = millis();
void refresh_lv_tick_value() {
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

void refresh_screen() {
  refresh_lv_tick_value();
  lv_timer_handler();
  wdtRefresh();
}

void setLoadingScreenText(const char* text) {
  logger.debug(text);
  lv_label_set_text(ui_LoadingLabel, text);
  refresh_screen();
}
