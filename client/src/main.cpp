// this must be first, even before debug.h
#include <Arduino.h>
#include <ArduinoOTA.h>

#include "secrets.h"
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

// then everything else can be included
#include "debug.h"
#include "main.h"
#include "ui/ui.h"

#ifdef USE_WDT
#include <esp_task_wdt.h>
#endif

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

void wdtInit() {
#ifdef USE_WDT
  TRACE_PRINT("Configuring WDT for %d seconds", WDT_TIMEOUT);
  esp_task_wdt_init(WDT_TIMEOUT, true);  // enable panic so ESP32 restarts
  esp_task_wdt_add(NULL);                // add current thread to WDT watch
#endif
}

void wdtRefresh() {
#ifdef USE_WDT
  TRACE_PRINT("(WDT ping)");
  esp_task_wdt_reset();
#endif
}

void wdtStop() {
#ifdef USE_WDT
  TRACE_PRINT("Stopping WDT...");
  esp_task_wdt_deinit();
#endif
}

void publish_homeassistant_value(bool startup,                // true if the device is starting up, false if the value is changing
                                 String component,            // component type e.g. "sensor", "text", "switch", etc.
                                 String device_topic,         // device topic name, e.g. "energy_monitor" or "monitor1"
                                 String key,                  // sensor name, e.g. "backlight_status"
                                 String value,                // initial value
                                 String entity_category,      // "diagnostic", "config", etc.
                                 String device_class,         // see https://www.home-assistant.io/integrations/sensor/#device-class
                                 String state_class,          // usually "measurement"
                                 String unit_of_measurement,  // e.g. "W", "V", "A", "kWh", etc.
                                 String icon                  // "mdi:battery" etc.
) {
  // use "sensor" if component is empty
  if (component == "") {
    component = "sensor";
  }

  String config_topic = "homeassistant/" + component + "/" + device_topic + "/" + key + "/config";
  String state_topic = "energy_monitor/" + device_topic + "/state/" + key;

  if (startup) {
    // create HomeAssistant config JSON
    JsonDocument doc;
    doc["state_topic"] = state_topic;
    doc["device"]["manufacturer"] = "Michal";
    doc["device"]["model"] = "Energy Monitor";
    doc["device"]["hw_version"] = "1.0";
    doc["device"]["sw_version"] = VERSION;

    JsonArray identifiers = doc["device"]["identifiers"].to<JsonArray>();
    identifiers.add(device_topic);
    doc["device"]["name"] = device_topic;
    doc["enabled_by_default"] = true;
    doc["entity_category"] = entity_category;
    doc["device_class"] = device_class;
    doc["state_class"] = state_class;
    doc["unit_of_measurement"] = unit_of_measurement;
    doc["icon"] = icon;
    // 2024-02-09 16:17:16.633 WARNING (MainThread) [homeassistant.components.mqtt.mixins] MQTT entity name starts with the device name in your config
    // {'state_topic': 'energy_monitor/energymonitor1/state/backlight', 'device': {'manufacturer': 'Michal', 'model': 'Energy Monitor', 'identifiers':
    // ['energymonitor1'], 'name': 'energymonitor1', 'connections': []}, 'enabled_by_default': True, 'entity_category': <EntityCategory.DIAGNOSTIC:
    // 'diagnostic'>, 'device_class': <BinarySensorDeviceClass.LIGHT: 'light'>, 'icon': 'mdi:lightbulb', 'name': 'energymonitor1 backlight', 'unique_id':
    // 'energymonitor1_backlight', 'force_update': False, 'qos': 0, 'availability_mode': 'latest', 'payload_on': 'ON', 'payload_not_available': 'offline',
    // 'encoding': 'utf-8', 'payload_off': 'OFF', 'payload_available': 'online'}, this is not expected. Please correct your configuration. The device name
    // prefix will be stripped off the entity name and becomes 'backlight'
    doc["name"] = key;  // fixed, see above^
    doc["unique_id"] = device_topic + "_" + key;

    String serialized;
    serializeJson(doc, serialized);
    mqttClient.publish(config_topic.c_str(), serialized.c_str(), true);  // publish as retained, to survive HA restart
  }
  mqttClient.publish(state_topic.c_str(), value.c_str(), false);
}

void publish_homeassistant_value_backlight(bool startup, int on_off) {
  publish_homeassistant_value(startup, "binary_sensor", MQTT_HA_DEVICENAME, "backlight", on_off ? "ON" : "OFF", "diagnostic",
                              "light", /* one of 'battery', 'battery_charging', 'carbon_monoxide', 'cold', 'connectivity', 'door', 'garage_door', 'gas', 'heat',
                                          'light', 'lock', 'moisture', 'motion', 'moving', 'occupancy', 'opening', 'plug', 'power', 'presence', 'problem',
                                          'running', 'safety', 'smoke', 'sound', 'tamper', 'update', 'vibration', 'window' */
                              "measurement", "", "mdi:lightbulb");
}

int last_warningstate = -1;
void publish_homeassistant_value_warningstate(bool startup, int on_off) {
  if (on_off == last_warningstate && !startup) {
    return;
  }
  publish_homeassistant_value(startup, "binary_sensor", MQTT_HA_DEVICENAME, "warning", on_off ? "ON" : "OFF", "diagnostic",
                              "power", /* one of 'battery', 'battery_charging', 'carbon_monoxide', 'cold', 'connectivity', 'door', 'garage_door', 'gas', 'heat',
                                          'light', 'lock', 'moisture', 'motion', 'moving', 'occupancy', 'opening', 'plug', 'power', 'presence', 'problem',
                                          'running', 'safety', 'smoke', 'sound', 'tamper', 'update', 'vibration', 'window' */
                              "measurement", "", "mdi:power-socket-eu");
  last_warningstate = on_off;
}

long last_uptime = -1;
void publish_homeassistant_value_uptime(bool startup) {
  long uptime = millis() / 1000;
  if ((uptime - last_uptime >= 30) || (uptime < last_uptime) || startup) {  // publish every minute or if the value overflowed
    publish_homeassistant_value(startup, "sensor", MQTT_HA_DEVICENAME, "uptime", String(uptime), "diagnostic", "duration", "measurement", "s",
                                "mdi:chart-box-outline");
    last_uptime = uptime;
  }
}

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

  publish_homeassistant_value_backlight(false, on_off);

  // DEBUG_PRINT("Setting backlight to %d", on_off);
  if (on_off == 0) {
    ledcWrite(BL_LEDC_CHANNEL, 0);
  } else {
    ledcWrite(BL_LEDC_CHANNEL, 255);
  }
  backlight_on = on_off;
}

void toggleBacklightManually() {
  // DEBUG_PRINT("Setting backlight manually to %d", !backlight_on);
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
  wdtRefresh();
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
    publish_homeassistant_value_warningstate(false, 1);
  } else {
    lv_disp_load_scr(ui_OKScreen);
    publish_homeassistant_value_warningstate(false, 0);
  }
}

void handleMQTTMessageConfiguration(String payloadString) {
  DeserializationError error = deserializeJson(config, payloadString);
  if (error) {
    DEBUG_PRINT("Parsing input failed: %s", error.c_str());
    setLoadingScreenText("Invalid config!");
    return;
  }

  // DEBUG_PRINT("Parsed configuration:");
  // String tmp = "";
  // serializeJsonPretty(config, tmp);
  // DEBUG_PRINT(tmp.c_str());

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

  wdtStop();
#ifdef USE_WIFI_MANAGER
  wifiManager.setHostname(NETWORK_HOSTNAME);
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
  wdtInit();


// #ifdef USE_WDT
//   if (esp_reset_reason() == ESP_RST_TASK_WDT) {
//     sleepTime = SLEEP_TIME_PERMANENT_ERROR;
//     error("Watchdog issue. Please report this to the developer.");
//   }
// #endif

  setLoadingScreenText("Connecting to MQTT");
  initMQTT();
  reconnectMQTT();

  setLoadingScreenText("Enabling OTA");
  // enable OTA
  ArduinoOTA.setHostname(NETWORK_HOSTNAME);
  ArduinoOTA.begin();
  ArduinoOTA.onStart([]() { DEBUG_PRINT("OTA Start"); wdtStop(); });
  ArduinoOTA.onEnd([]() { DEBUG_PRINT("OTA End"); });

  setLoadingScreenText("Publishing HA MQTT config");
  publish_homeassistant_value_backlight(true, 0);
  publish_homeassistant_value_uptime(true);
  publish_homeassistant_value_warningstate(true, 0);

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
    DEBUG_PRINT("No MQTT message arrived for %d s, connected=%d, state=%d", mqttTimeout.limitMillis() / SECONDS_TO_MILLIS, mqttClient.connected(),
                mqttClient.state());

    if (WiFi.status() == WL_CONNECTED && !mqttClient.connected()) {
      DEBUG_PRINT("MQTT not connected, reconnecting");
      reconnectMQTT();
    } else {
      DEBUG_PRINT("Fully rebooting after 60 seconds", mqttTimeout.limitMillis() / SECONDS_TO_MILLIS);
      delay(60 * SECONDS_TO_MILLIS);
      DEBUG_PRINT("rebooting now");
      ESP.restart();
    }
  }
  refresh_screen();
  publish_homeassistant_value_uptime(false);
  delay(5 * MILLIS);
}
