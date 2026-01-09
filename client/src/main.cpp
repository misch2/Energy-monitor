// this must be first, even before debug.h
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WiFi.h>
#include <WiFiManager.h>

#include "secrets.h"

// clang-format off
#include "lv_conf.h"
#include <lvgl.h>
// clang-format on

// then everything else can be included
#include "main.h"
#include "ui/ui.h"

// Include modular headers
#include "backlight.h"
#include "debug.h"
#include "display.h"
#include "homeassistant.h"
#include "led.h"
#include "mqtt.h"
#include "power.h"
#include "system.h"

#define SECONDS_TO_MILLIS 1000
#define MILLIS 1

WiFiManager wifiManager;
WiFiClient wifiClient;
WiFiUDP udpClient;
// Create a new syslog instance with LOG_KERN facility
Syslog syslog(udpClient, SYSLOG_SERVER, SYSLOG_PORT, SYSLOG_MYHOSTNAME, SYSLOG_MYAPPNAME, LOG_KERN);
Logger logger;

void test1() {
  logger.debug("here 1");
  lv_disp_load_scr(ui_OKScreen);
  refresh_screen();
  // logger.debug("a");
  delay(2 * SECONDS_TO_MILLIS);

  logger.debug("here 2");
  lv_disp_load_scr(ui_WarningScreen);
  refresh_screen();
  lv_timer_handler();
  delay(2 * SECONDS_TO_MILLIS);

  logger.debug("here 3");
  setLoadingScreenText("Random text");
  delay(2 * SECONDS_TO_MILLIS);

  logger.debug("here 4");
  setLoadingScreenText("Foobar");
  delay(2 * SECONDS_TO_MILLIS);

  logger.debug("end");
}

void test2() {
  logger.debug("initDisplay");
  initDisplay();
  logger.debug("initLVGL");
  initLVGL();
  logger.debug("ui_init");
  ui_init();
  logger.debug("initDisplay done, entering infinite loop");
  for (int i = 0; i < 10000; i++) {
    logger.debug("in the loop, i=%d", i);
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

  initDisplay();
  initLVGL();
  ui_init();

  setLoadingScreenText("Connecting to WiFi");

  wdtStop();
  wifiManager.setHostname(NETWORK_HOSTNAME);
  if (!wifiManager.getWiFiIsSaved()) {
    setLoadingScreenText("Starting WiFi AP!");
  }
  wifiManager.autoConnect();
  logResetReason();
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
  ArduinoOTA.onStart([]() {
    logger.debug("OTA Start");

    wdtStop();

    setBacklight(1);
    backlightTimeout.stop();

    lv_disp_load_scr(ui_LoadingScreen);
    setLoadingScreenText("OTA in progress...");
    refresh_screen();
  });
  ArduinoOTA.onEnd([]() { logger.debug("OTA End"); });

  setLoadingScreenText("Publishing HA MQTT config");
  publish_homeassistant_value_backlight(true, 0);
  publish_homeassistant_value_uptime(true);
  publish_homeassistant_value_warningstate(true, 0);

  setLoadingScreenText("Loading configuration");
  while (mqttTopicCurrentPower == "") {
    loop();
  }

  setLoadingScreenText("Initialization done");

  updateCurrentPower();
  lv_disp_load_scr(ui_OKScreen);
  refresh_screen();

  logger.debug("Setup done, version %s", VERSION);
}

void loop() {
  ArduinoOTA.handle();
  mqttClient.loop();
  if (backlightTimeout.expired()) {
    setBacklight(0);
    backlightTimeout.stop();
  }
  if (mqttTimeout.expired()) {
    // logger.debug("MQTT timeout, reconnecting");
    // mqttClient.disconnect();
    // mqttTimeout.stop();
    logger.debug("No MQTT message arrived for %d s, connected=%d, state=%d", mqttTimeout.limitMillis() / SECONDS_TO_MILLIS, mqttClient.connected(),
                 mqttClient.state());

    if (WiFi.status() == WL_CONNECTED && !mqttClient.connected()) {
      logger.debug("MQTT not connected, reconnecting");
      reconnectMQTT();
    } else {
      logger.debug("Fully rebooting after 60 seconds", mqttTimeout.limitMillis() / SECONDS_TO_MILLIS);
      delay(60 * SECONDS_TO_MILLIS);
      logger.debug("rebooting now");
      ESP.restart();
    }
  }
  refresh_screen();
  publish_homeassistant_value_uptime(false);
  delay(5 * MILLIS);
}
