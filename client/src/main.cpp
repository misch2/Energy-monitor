#include <Arduino.h>
#include <ArduinoOTA.h>
// #include <Arduino_JSON.h>
#include <ArduinoJson.h>
#include <Array.h>
#include <PubSubClient.h>
#include <Syslog.h>
#include <TAMC_GT911.h>
#include <TFT_eSPI.h>
#include <Timemark.h>
#include <WiFi.h>

#include <CircularBuffer.hpp>
#ifdef USE_WIFI_MANAGER
#include <WiFiManager.h>
#endif

// clang-format off
#include "lv_conf.h"
#include <lvgl.h>
// clang-format on

// this must be first, even before debug.h
#include "secrets.h"
// then everything else can be included
#include "debug.h"
#include "main.h"
#include "ui/ui.h"

#define LED_PIN_RED 4
#define LED_PIN_GREEN 16
#define LED_PIN_BLUE 17

#define BL_LEDC_PIN 27
#define BL_LEDC_CHANNEL 0

#define SECONDS_TO_MILLIS 1000
#define MILLIS 1

// landscape, USB on the right
// #define ROTATE_TFT 1
// #define ROTATE_TOUCH ROTATION_RIGHT

// portrait, USB at the bottom
#define ROTATE_TFT 0
#define ROTATE_TOUCH ROTATION_INVERTED  // NORMAL, LEFT, INVERTED, RIGHT

#define MAX_APPLIANCES 12  // as in the UI files (number of the last ui_LabelApplianceXX)

/* LOGICAL screen orientation (i.e. rotation dependent) */
static const uint16_t screenWidth = 320;
static const uint16_t screenHeight = 480;

static lv_disp_draw_buf_t draw_buf;
#define DRAW_BUF_SIZE (screenWidth * screenHeight / 10)
static lv_color_t buf[DRAW_BUF_SIZE];

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */
TAMC_GT911 tp = TAMC_GT911(TOUCH_SDA, TOUCH_SCL, TOUCH_INT, TOUCH_RST, screenWidth, screenHeight);
#ifdef USE_WIFI_MANAGER
WiFiManager wifiManager;
#endif
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

JsonDocument config;
// parsed values from config
String mqttTopicCurrentPower = "";
int voltage = 0;
int maxCurrent = 0;
int limitWatts = 0;

const int MAX_POWER_READINGS_BUFFER = 10;
CircularBuffer<float, MAX_POWER_READINGS_BUFFER> powerReadingsBuffer;

#if LV_USE_LOG != 0
/* Serial debugging */
// void my_print( lv_log_level_t level, const char * buf )
void my_print(const char* buf) {
  // LV_UNUSED(level);
  DEBUG_PRINT(buf);
}
#endif

/* Display flushing */
void my_disp_flush(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  // DEBUG_PRINT("Flushing area: x1=%d, y1=%d, x2=%d, y2=%d, w=%d, h=%d", area->x1, area->y1, area->x2, area->y2, w, h);
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

int backlight_on = 0;
Timemark backlightTimeout(30 * SECONDS_TO_MILLIS);
Timemark mqttTimeout(60 * SECONDS_TO_MILLIS);

void setBacklight(int on_off) {  // 0 - 255
  if (backlight_on == on_off) {
    return;
  }

  DEBUG_PRINT("Setting backlight to %d", on_off);
  if (on_off == 0) {
    ledcWrite(BL_LEDC_CHANNEL, 0);
  } else {
    ledcWrite(BL_LEDC_CHANNEL, 255);
  }
  backlight_on = on_off;
}

void toggleBacklightManually() {
  DEBUG_PRINT("Setting backlight manually to %d", !backlight_on);
  setBacklight(!backlight_on);
  backlightTimeout.stop();
}

void initBacklight() {
  // PWM backlight on PIN 27
  pinMode(BL_LEDC_PIN, OUTPUT);
  ledcAttachPin(BL_LEDC_PIN, BL_LEDC_CHANNEL);
  ledcSetup(BL_LEDC_CHANNEL, 5000, 8);
  setBacklight(1);
  backlightTimeout.start();  // turn off backlight after 30 seconds
}

void initDisplay() {
  turnOffAllLEDs();
  initBacklight();

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
}

void setLoadingScreenText(const char* text) {
  DEBUG_PRINT(text);
  lv_label_set_text(ui_LoadingLabel, text);
  refresh_screen();
}

bool showApplianceLabel(lv_obj_t* ui_element, JsonArray appliances, int number, int remainingWatts) {
  // DEBUG_PRINT("in showApplianceLabel(%d, %d)", number, remainingWatts);

  if (number > appliances.size()) {
    // DEBUG_PRINT("Appliance #%d not found in config, skipping this label", number);
    lv_obj_add_flag(ui_element, LV_OBJ_FLAG_HIDDEN);
    return false;
  }

  JsonVariant appliance = appliances[number - 1];
  if (remainingWatts > (int)appliance["power"]) {
    // DEBUG_PRINT("Appliance %d compliant, remainingWatts %d > %d, skipping this label", number, remainingWatts, (int)appliance["power"]);
    lv_obj_add_flag(ui_element, LV_OBJ_FLAG_HIDDEN);
    return false;
  };

  String name = (const char*)appliance["name"]["accusative"];
  // DEBUG_PRINT("Appliance %d displayed, remainingWatts %d <= %d, name: %s", number, remainingWatts, (int)appliance["power"], name.c_str());
  lv_label_set_text(ui_element, name.c_str());
  lv_obj_clear_flag(ui_element, LV_OBJ_FLAG_HIDDEN);

  return true;
}

/* FIXME current_power:
lv_label_set_text(ui_Label3, config["display"]["current_power"]["name"].as<char*>()]);
lv_label_set_text(ui_Label2,dtto)

Dtto for remaining_power


unit
name
round_to_multiple_of


*/

void handleMQTTMessageCurrentPower(String payloadString) {
  float currentWatts = payloadString.toFloat();

  // add currentWatts to buffer and find max over last MAX_POWER_READINGS_BUFFER readings
  powerReadingsBuffer.push(currentWatts);
  float filteredCurrentWatts = currentWatts;
  for (decltype(powerReadingsBuffer)::index_t i = 0; i < powerReadingsBuffer.size(); i++) {
    if (powerReadingsBuffer[i] > filteredCurrentWatts) {
      filteredCurrentWatts = powerReadingsBuffer[i];
    }
  };

  float remainingWatts = limitWatts - filteredCurrentWatts;       // time filtered and rounded down value for the warning message, to prevent flickering
  int displayedRemainingWatts = (int)(remainingWatts / 50) * 50;  // round down to multiples of 50

  // non-filtered and more precise value for the gauge
  lv_arc_set_value(ui_ArcCurrentWattsOK, currentWatts);
  lv_arc_set_value(ui_ArcCurrentWattsWarning, currentWatts);

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

  JsonArray appliances = config["electricity"]["appliances"].as<JsonArray>();
  if (appliances.size() > MAX_APPLIANCES) {
    // DEBUG_PRINT("Too many appliances in config, only %d are supported", MAX_APPLIANCES);
    // The total number of appliances is not an issue. It would be a problem only if all of them should be displayed at the same time.
  }

  bool displayed_warning = false;
  displayed_warning |= showApplianceLabel(ui_LabelAppliance1, appliances, 1, displayedRemainingWatts);
  displayed_warning |= showApplianceLabel(ui_LabelAppliance2, appliances, 2, displayedRemainingWatts);
  displayed_warning |= showApplianceLabel(ui_LabelAppliance3, appliances, 3, displayedRemainingWatts);
  displayed_warning |= showApplianceLabel(ui_LabelAppliance4, appliances, 4, displayedRemainingWatts);
  displayed_warning |= showApplianceLabel(ui_LabelAppliance5, appliances, 5, displayedRemainingWatts);
  displayed_warning |= showApplianceLabel(ui_LabelAppliance6, appliances, 6, displayedRemainingWatts);
  displayed_warning |= showApplianceLabel(ui_LabelAppliance7, appliances, 7, displayedRemainingWatts);
  displayed_warning |= showApplianceLabel(ui_LabelAppliance8, appliances, 8, displayedRemainingWatts);
  displayed_warning |= showApplianceLabel(ui_LabelAppliance9, appliances, 9, displayedRemainingWatts);
  displayed_warning |= showApplianceLabel(ui_LabelAppliance10, appliances, 10, displayedRemainingWatts);
  displayed_warning |= showApplianceLabel(ui_LabelAppliance11, appliances, 11, displayedRemainingWatts);
  displayed_warning |= showApplianceLabel(ui_LabelAppliance12, appliances, 12, displayedRemainingWatts);

  if (displayed_warning) {
    lv_disp_load_scr(ui_WarningScreen);
    setBacklight(1);
    backlightTimeout.start();  // turn off backlight after 30 seconds
  } else {
    lv_disp_load_scr(ui_OKScreen);
  }
}

void handleMQTTMessageConfiguration(String payloadString) {
  DeserializationError error = deserializeJson(config, payloadString);
  if (error) {
    DEBUG_PRINT("Parsing input failed: %s", error.c_str());
    setLoadingScreenText("Invalid config!");
    return;
  }

  DEBUG_PRINT("Parsed configuration:");
  String tmp = "";
  serializeJsonPretty(config, tmp);
  DEBUG_PRINT(tmp.c_str());

  voltage = (int)config["electricity"]["meter"]["voltage"];
  maxCurrent = (int)config["electricity"]["meter"]["current"];
  limitWatts = voltage * maxCurrent;
  lv_arc_set_range(ui_ArcCurrentWattsOK, 0, limitWatts);
  lv_arc_set_range(ui_ArcCurrentWattsWarning, 0, limitWatts);

  mqttTopicCurrentPower = (const char*)config["topics"]["current_power"];
  DEBUG_PRINT("Subscribing to power meter topic [%s]", mqttTopicCurrentPower.c_str());
  bool ok = mqttClient.subscribe(mqttTopicCurrentPower.c_str());
  if (!ok) {
    setLoadingScreenText("MQTT subscription failed!");
  }
}

// handle message arrived
void MQTTcallback(char* topic, byte* payload, unsigned int length) {
  String topicString = String(topic);
  String payloadString = "";
  for (int i = 0; i < length; i++) {
    payloadString += (char)payload[i];
  }

  // DEBUG_PRINT("Message arrived on topic [" + topicString + "] Payload: [" + payloadString + "]");

  if (topicString.equals(mqttTopicCurrentPower)) {
    handleMQTTMessageCurrentPower(payloadString);
  } else if (topicString.equals(MQTT_CONFIGURATION_TOPIC)) {
    handleMQTTMessageConfiguration(payloadString);
  } else {
    DEBUG_PRINT("Unknown topic [%s]", topicString.c_str());
  }

  mqttTimeout.start();  // reset the timeout
}

void initMQTT() {
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(MQTTcallback);
}

void reconnectMQTT() {
  if (mqttClient.connected()) {
    return;
  }

  while (!mqttClient.connected()) {
    DEBUG_PRINT("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect("EnergyMonitor/1.0", MQTT_USER, MQTT_PASSWORD)) {
      DEBUG_PRINT(" - connected");
      DEBUG_PRINT("Subscribing to configuration topic [%s]", MQTT_CONFIGURATION_TOPIC);
      bool ok = mqttClient.subscribe(MQTT_CONFIGURATION_TOPIC);
      // DEBUG_PRINT(ok ? "Subscribed" : "Subscription failed");
    } else {
      DEBUG_PRINT(" - failed, rc=%d", mqttClient.state());
      DEBUG_PRINT(" - trying again in 5 seconds");
      delay(5 * SECONDS_TO_MILLIS);
    }
  }

  mqttTimeout.start();
}

void test1() {
  DEBUG_PRINT("here 1");
  lv_disp_load_scr(ui_OKScreen);
  refresh_screen();
  // DEBUG_PRINT("a");
  delay(2 * SECONDS_TO_MILLIS);

  DEBUG_PRINT("here 2");
  lv_disp_load_scr(ui_WarningScreen);
  refresh_screen();
  lv_timer_handler();
  delay(2 * SECONDS_TO_MILLIS);

  DEBUG_PRINT("here 3");
  setLoadingScreenText("Random text");
  delay(2 * SECONDS_TO_MILLIS);

  DEBUG_PRINT("here 4");
  setLoadingScreenText("Foobar");
  delay(2 * SECONDS_TO_MILLIS);

  DEBUG_PRINT("end");
}

void test2() {
  DEBUG_PRINT("initDisplay");
  initDisplay();
  DEBUG_PRINT("initLVGL");
  initLVGL();
  DEBUG_PRINT("ui_init");
  ui_init();
  DEBUG_PRINT("initDisplay done, entering infinite loop");
  for (int i = 0; i < 10000; i++) {
    DEBUG_PRINT("in the loop, i=%d", i);
    lv_disp_load_scr(ui_OKScreen);
    lv_label_set_text(ui_LabelRemainingWattsOK, i % 2 == 0 ? "even" : "odd");
    refresh_screen();
    delay(2 * SECONDS_TO_MILLIS);
    lv_disp_load_scr(ui_WarningScreen);
    refresh_screen();
    delay(2 * SECONDS_TO_MILLIS);
  }
}

void setup() {
  Serial.begin(115200); /* prepare for possible serial debug */

  powerReadingsBuffer.clear();
  initDisplay();
  initLVGL();
  ui_init();

  setLoadingScreenText("Connecting to WiFi");

#ifdef USE_WIFI_MANAGER
  if (!wifiManager.getWiFiIsSaved()) {
    setLoadingScreenText("Starting WiFi AP!");
  }
  wifiManager.autoConnect();
#else
  WiFi.setHostname(NETWORK_HOSTNAME);
  WiFi.config(NETWORK_IP_ADDRESS, NETWORK_IP_GATEWAY, NETWORK_IP_SUBNET, NETWORK_IP_DNS);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    refresh_screen();
    delay(5 * MILLIS);
  }
#endif

  setLoadingScreenText("Connecting to MQTT");
  initMQTT();
  reconnectMQTT();

  setLoadingScreenText("Enabling OTA");
// enable OTA
#ifndef USE_WIFI_MANAGER
  ArduinoOTA.setHostname(NETWORK_HOSTNAME);
#endif
  ArduinoOTA.begin();

  setLoadingScreenText("Loading configuration");
  while (mqttTopicCurrentPower == "") {
    loop();
  }

  setLoadingScreenText("Initialization done");

  handleMQTTMessageCurrentPower("0");
  lv_disp_load_scr(ui_OKScreen);
  refresh_screen();

  DEBUG_PRINT("Setup done, version %s", VERSION);
}

void loop() {
  ArduinoOTA.handle();
  mqttClient.loop();
  if (backlightTimeout.expired()) {
    setBacklight(0);
    backlightTimeout.stop();
  }
  if (mqttTimeout.expired()) {
    // DEBUG_PRINT("MQTT timeout, reconnecting");
    // mqttClient.disconnect();
    // mqttTimeout.stop();
    // reconnectMQTT();
    DEBUG_PRINT("No MQTT message arrived for %d s, rebooting after 60 seconds", mqttTimeout.limitMillis() / SECONDS_TO_MILLIS);
    delay(60 * SECONDS_TO_MILLIS);
    DEBUG_PRINT("rebooting now");
    ESP.restart();
  }
  refresh_screen();
  delay(5 * MILLIS);
}
