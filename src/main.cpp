
#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <lvgl.h>

#define LED_PIN_RED 4
#define LED_PIN_GREEN 16
#define LED_PIN_BLUE 17

#define BL_LEDC_PIN 27
#define BL_LEDC_CHANNEL 0

TFT_eSPI tft = TFT_eSPI();

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

void my_disp_flush(lv_disp_drv_t* disp_drv,
                   const lv_area_t* area,
                   lv_color_t* color_p) {
  uint16_t c;

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, area->x2, area->y2);

  for (int y = area->y1; y <= area->y2; y++) {
    for (int x = area->x1; x <= area->x2; x++) {
      c = color_p->full;
      tft.writeColor(c, 1);
      color_p++;
    }
  }

  tft.endWrite();

  lv_disp_flush_ready(disp_drv);

  if (lv_disp_flush_is_last(disp_drv)) {
    // Do any necessary cleanup here
  }
}

void initLVGL() {
  initBacklight();
  setBacklight(255);

  lv_init();

  tft.init();

  Serial.println("here a1");
  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(ILI9341_CYAN);  // FIXME black
  tft.println("Starting...");
  delay(1000);
  tft.println("foobar");

  Serial.println("here a5");

  lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.flush_cb = my_disp_flush;
  lv_disp_drv_register(&disp_drv);
  Serial.println("here a6");

  lv_obj_t* scr = lv_obj_create(NULL);
  lv_scr_load(scr);
  Serial.println("here a7");

  lv_obj_t* label = lv_label_create(scr);
  lv_label_set_text(label, "Hello, world!");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  Serial.println("here a8");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("here 1");
  turnOffAllLEDs();
  Serial.println("here 2");
  initLVGL();
  Serial.println("here 3");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("task handler");
  // FIXME lv_task_handler();
  // delay(5);
  delay(5000);
}
