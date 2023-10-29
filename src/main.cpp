#include <Arduino.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <TAMC_GT911.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <lvgl.h>

#include "secrets.h"

#define LED_PIN_RED 4
#define LED_PIN_GREEN 16
#define LED_PIN_BLUE 17

#define BL_LEDC_PIN 27
#define BL_LEDC_CHANNEL 0

/*Change to your screen resolution*/
static const uint16_t screenWidth = 480;
static const uint16_t screenHeight = 320;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight / 10];

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */
TAMC_GT911 tp = TAMC_GT911(TOUCH_SDA, TOUCH_SCL, TOUCH_INT, TOUCH_RST, screenWidth, screenHeight);
WiFiManager wifiManager;
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

#if LV_USE_LOG != 0
/* Serial debugging */
// void my_print( lv_log_level_t level, const char * buf )
void my_print(const char* buf) {
  // LV_UNUSED(level);
  Serial.println(buf);
  Serial.flush();
}
#endif

/* Display flushing */
void my_disp_flush(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t*)&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp_drv);
}

// touch read:
// tp.read();
// if (tp.isTouched) {
//   for (int i = 0; i < tp.touches; i++) {
//     Serial.print("Touch ");
//     Serial.print(i + 1);
//     Serial.print(": ");
//     ;
//     Serial.print("  x: ");
//     Serial.print(tp.points[i].x);
//     Serial.print("  y: ");
//     Serial.print(tp.points[i].y);
//     Serial.print("  size: ");
//     Serial.println(tp.points[i].size);
//     Serial.println(' ');
//   }
// }

/*Read the touchpad*/
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

void turnOffLED(int led_pin) {
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, HIGH);
}

void turnOnLED(int led_pin) {
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);
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

  tp.begin();
  tp.setRotation(ROTATION_RIGHT);
}

void initLVGL() {
  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

  Serial.println(LVGL_Arduino);
  Serial.println("I am LVGL_Arduino");

  lv_init();

#if LV_USE_LOG != 0
  lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

  tft.begin();        /* TFT init */
  tft.setRotation(1); /* Landscape orientation */
  // tft.setRotation(3); /* Landscape orientation, flipped */

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

static void event_handler(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);

  if (code == LV_EVENT_CLICKED) {
    Serial.println("Clicked");
  } else if (code == LV_EVENT_VALUE_CHANGED) {
    Serial.println("Toggled");
  }
}

void MQTTcallback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
  Serial.print("Message arrived [");

  String payloadString = "";
  for (int i = 0; i < length; i++) {
    payloadString += (char)payload[i];
  }

  Serial.print(topic);
  Serial.print("] ");
  Serial.println(payloadString);
  Serial.println();
}

void initMQTT() {
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(MQTTcallback);  // FIXME name
}

void reconnectMQTT() {
  if (mqttClient.connected()) {
    return;
  }

  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect("arduinoClient", MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // mqttClient.publish("outTopic", "hello world");
      // ... and resubscribe
      mqttClient.subscribe(MQTT_TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200); /* prepare for possible serial debug */

  initDisplay();
  initLVGL();

  lv_obj_t* scr = lv_scr_act();
  lv_obj_t* label2 = lv_label_create(scr);
  lv_obj_align(label2, LV_ALIGN_CENTER, 0, 100);

  lv_label_set_text(label2, "Connecting to WiFi");
  lv_timer_handler(); /* let the GUI do its work */
  Serial.println("Connecting to WiFi");
  wifiManager.autoConnect();
  Serial.println("Connected to WiFi");
  delay(1000);

  lv_label_set_text(label2, "Connecting to MQTT");
  lv_timer_handler(); /* let the GUI do its work */
  Serial.println("Connecting to MQTT");
  initMQTT();
  reconnectMQTT();

  // enable OTA
  // ArduinoOTA.setHostname("esp32");
  lv_label_set_text(label2, "Enabling OTA");
  ArduinoOTA.begin();
  Serial.println("OTA ready");

  lv_obj_del(label2);

  lv_obj_t* button = lv_btn_create(scr);
  lv_obj_align(button, LV_ALIGN_CENTER, 0, 130);
  lv_obj_set_size(button, 100, 50);
  lv_obj_add_event_cb(button, event_handler, LV_EVENT_ALL, NULL);

  lv_obj_t* label3 = lv_label_create(button);
  lv_label_set_text(label3, "Tlacitko");
  lv_obj_align(label3, LV_ALIGN_CENTER, 0, 0);

  Serial.println("Setup done");
}

void loop() {
  lv_timer_handler(); /* let the GUI do its work */
  ArduinoOTA.handle();
  mqttClient.loop();
  delay(5);
}
