#include "main.h"

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <TAMC_GT911.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <lvgl.h>

#include "main.h"
#include "secrets.h"
#include "ui/ui.h"

#define LED_PIN_RED 4
#define LED_PIN_GREEN 16
#define LED_PIN_BLUE 17

#define BL_LEDC_PIN 27
#define BL_LEDC_CHANNEL 0

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
static lv_color_t buf[screenWidth * screenHeight / 10];

static const int VOLTAGE = 230;
static const int MAX_CURRENT = 25;
static const int MAX_WATTS = VOLTAGE * MAX_CURRENT;

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

int backlight_on;

void setBacklight(int on_off) {  // 0 - 255
  Serial.print("Setting backlight to ");
  Serial.println(on_off);

  if (on_off == 0) {
    ledcWrite(BL_LEDC_CHANNEL, 0);
  } else {
    ledcWrite(BL_LEDC_CHANNEL, 255);
  }
  backlight_on = on_off;
}

void toggleBacklight() { setBacklight(!backlight_on); }

void initBacklight() {
  // PWM backlight on PIN 27
  pinMode(BL_LEDC_PIN, OUTPUT);
  ledcAttachPin(BL_LEDC_PIN, BL_LEDC_CHANNEL);
  ledcSetup(BL_LEDC_CHANNEL, 5000, 8);
  backlight_on = 0;
  setBacklight(1);
}

void initDisplay() {
  turnOffAllLEDs();
  initBacklight();

  tp.begin();
  tp.setRotation(ROTATE_TOUCH);
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

  tft.begin(); /* TFT init */
  tft.setRotation(ROTATE_TFT);

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
}

void refresh_screen() { lv_timer_handler(); }

void setLoadingScreenText(const char* text) {
  Serial.println(text);
  lv_label_set_text(ui_LoadingLabel, text);
  refresh_screen();
}

static void event_handler(lv_event_t* e) {
  lv_event_code_t code = lv_event_get_code(e);

  if (code == LV_EVENT_CLICKED) {
    Serial.println("Clicked");
  } else if (code == LV_EVENT_VALUE_CHANGED) {
    Serial.println("Toggled");
  }
}

void handleMQTTMessageCurrentPower(String payloadString) {
  float currentWatts = payloadString.toFloat();
  float remainingWatts = MAX_WATTS - currentWatts;
  // round down to multiples of 50
  int displayedRemainingWatts = (int)(remainingWatts / 50) * 50;

  lv_arc_set_value(ui_ArcCurrentWatts, currentWatts);

  String label = "";
  label += displayedRemainingWatts;
  label += " W";
  lv_label_set_text(ui_LabelRezervaWattu, label.c_str());

  label = "";
  label += (int)(currentWatts);
  label += " W";
  lv_label_set_text(ui_LabelWattsUsed, label.c_str());

  // show green "OK" background and hide the red "Warning" background
  if (displayedRemainingWatts == 5500) {  // FIXME test only
    lv_obj_clear_flag(ui_PanelTopWarning, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_PanelTopOK, LV_OBJ_FLAG_HIDDEN);
  } else {
    lv_obj_clear_flag(ui_PanelTopOK, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_PanelTopWarning, LV_OBJ_FLAG_HIDDEN);
  }
}

// handle message arrived
void MQTTcallback(char* topic, byte* payload, unsigned int length) {
  // convert payload to string
  String payloadString = "";
  for (int i = 0; i < length; i++) {
    payloadString += (char)payload[i];
  }

  String topicString = String(topic);

  // Serial.print("Message arrived [");
  // Serial.print(topicString);
  // Serial.print("] ");
  // Serial.print(payloadString);
  // Serial.println();

  if (topicString.equals(MQTT_CURRENT_POWER_TOPIC)) {
    handleMQTTMessageCurrentPower(payloadString);
  } else {
    Serial.print("Unknown topic: " + topicString);
  }
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
      mqttClient.subscribe(MQTT_CURRENT_POWER_TOPIC);
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
  ui_init();

  setLoadingScreenText("Connecting to WiFi");
  wifiManager.autoConnect();

  setLoadingScreenText("Connecting to MQTT");
  initMQTT();
  reconnectMQTT();

  setLoadingScreenText("Enabling OTA");
  // enable OTA
  // ArduinoOTA.setHostname("esp32");
  ArduinoOTA.begin();

  setLoadingScreenText("Initialization done");
  delay(1000);

  lv_arc_set_range(ui_ArcCurrentWatts, 0, MAX_WATTS);
  lv_disp_load_scr(ui_OKScreen);
}

void loop() {
  lv_timer_handler(); /* let the GUI do its work */
  ArduinoOTA.handle();
  mqttClient.loop();
  delay(5);
}
