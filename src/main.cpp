#include <Arduino.h>
#include <WiFi.h>

#define LED_PIN_RED 4
#define LED_PIN_GREEN 16
#define LED_PIN_BLUE 17

#define BL_LEDC_PIN 27
#define BL_LEDC_CHANNEL 0

/*Using LVGL with Arduino requires some extra steps:
 *Be sure to read the docs here:
 *https://docs.lvgl.io/master/get-started/platforms/arduino.html  */

#include <TFT_eSPI.h>
#include <lvgl.h>

/*To use the built-in examples and demos of LVGL uncomment the includes below
 *respectively. You also need to copy `lvgl/examples` to `lvgl/src/examples`.
 *Similarly for the demos `lvgl/demos` to `lvgl/src/demos`. Note that the
 *`lv_examples` library is for LVGL v7 and you shouldn't install it for this
 *version (since LVGL v8) as the examples and demos are now part of the main
 *LVGL library. */

// #include <examples/lv_examples.h>
#include <demos/lv_demos.h>

/*Change to your screen resolution*/
static const uint16_t screenWidth = 480;
static const uint16_t screenHeight = 320;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight / 10];

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */

#if LV_USE_LOG != 0
/* Serial debugging */
// void my_print( lv_log_level_t level, const char * buf )
void my_print(const char* buf) {
  // LV_UNUSED(level);
  Serial.flush();
}
#endif

/* Display flushing */
void my_disp_flush(lv_disp_drv_t* disp_drv,
                   const lv_area_t* area,
                   lv_color_t* color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t*)&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp_drv);
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_drv_t* indev_driver, lv_indev_data_t* data) {
  uint16_t touchX, touchY;

  // bool touched = tft.getTouch( &touchX, &touchY, 600 );

  // if( !touched )
  // {
  data->state = LV_INDEV_STATE_REL;
  // }
  // else
  // {
  //     data->state = LV_INDEV_STATE_PR;

  //     /*Set the coordinates*/
  //     data->point.x = touchX;
  //     data->point.y = touchY;

  //     Serial.print( "Data x " );
  //     Serial.println( touchX );

  //     Serial.print( "Data y " );
  //     Serial.println( touchY );
  // }
}

void turnOffLED(int led_pin) {
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, HIGH);
}

void turnOffAllLEDs() {
  turnOffLED(LED_PIN_RED);
  turnOffLED(LED_PIN_GREEN);
  turnOffLED(LED_PIN_BLUE);
}

void initBacklight() {
  // PWM backlight on PIN 27
  pinMode(BL_LEDC_PIN, OUTPUT);
  ledcAttachPin(BL_LEDC_PIN, BL_LEDC_CHANNEL);
  ledcSetup(BL_LEDC_CHANNEL, 5000, 8);
}

void setBacklight(int brightness) {  // 0 - 255
  ledcWrite(BL_LEDC_CHANNEL, brightness);
}

void initDisplay() {
  turnOffAllLEDs();
  initBacklight();
  setBacklight(255);
}

void initLVGL() {
  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() +
                  "." + lv_version_patch();

  Serial.println(LVGL_Arduino);
  Serial.println("I am LVGL_Arduino");

  lv_init();

#if LV_USE_LOG != 0
  lv_log_register_print_cb(
      my_print); /* register print function for debugging */
#endif

  tft.begin();        /* TFT init */
  tft.setRotation(3); /* Landscape orientation, flipped */

  /*Set the touchscreen calibration data,
   the actual data for your display can be acquired using
   the Generic -> Touch_calibrate example from the TFT_eSPI library*/
  // uint16_t calData[5] = { 275, 3620, 264, 3532, 1 };
  // tft.setTouch( calData );

  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenHeight / 10);

  /*Initialize the display*/
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

  /* Create simple label */
  lv_obj_t* label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, LVGL_Arduino.c_str());
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t* scr = lv_scr_act();
  lv_obj_t* label2 = lv_label_create(scr);
  lv_label_set_text(label2, "Hello, world!");
  lv_obj_align(label2, LV_ALIGN_CENTER, 0, 50);
}

void setup() {
  Serial.begin(115200); /* prepare for possible serial debug */

  initDisplay();
  initLVGL();

  Serial.println("Setup done");
}

void loop() {
  lv_timer_handler(); /* let the GUI do its work */
  delay(5);
}
