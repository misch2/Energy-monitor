// this must be first, even before debug.h
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include <Timemark.h>
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
#include "appliance.h"
#include "backlight.h"
#include "demo_mode.h"
#include "display.h"
#include "homeassistant.h"
#include "led.h"
#include "logger.h"
#include "mqtt.h"
#include "power.h"
#include "system.h"
#include "ui_colors.h"

#define SECONDS_TO_MILLIS 1000
#define MILLIS 1

WiFiManager wifiManager;
WiFiClient wifiClient;
WiFiUDP udpClient;
LED leds;
Syslog syslog(udpClient, SYSLOG_SERVER, SYSLOG_PORT, SYSLOG_MYHOSTNAME, SYSLOG_MYAPPNAME, LOG_KERN);
Logger logger(udpClient, syslog);
Backlight backlight(logger);
PubSubClient mqttClient(wifiClient);
HomeAssistant homeassistant(mqttClient);
MQTTClientWrapper mqttWrapper(mqttClient, logger);
SystemLayer systemLayer(logger);
ApplianceList appliances;
UIColors uiColors;
Display display(leds, backlight, logger, systemLayer, uiColors);
Timemark displayRecalculate(1 * SECONDS_TO_MILLIS);
ElectricityMeter mainMeter;

JsonDocument jsonConfig;
String mqttTopicMainElectricityMeterPower = "";
DemoMode demo(logger, appliances, mainMeter);

Timemark safeModeDelayTimer(30 * SECONDS_TO_MILLIS);

void test1() {
  logger.debug("here 1");
  lv_disp_load_scr(ui_OKScreen);
  display.loop();
  // logger.debug("a");
  delay(2 * SECONDS_TO_MILLIS);

  logger.debug("here 2");
  lv_disp_load_scr(ui_WarningScreen);
  display.loop();
  lv_timer_handler();
  delay(2 * SECONDS_TO_MILLIS);

  logger.debug("here 3");
  display.setLoadingScreenText("Random text");
  delay(2 * SECONDS_TO_MILLIS);

  logger.debug("here 4");
  display.setLoadingScreenText("Foobar");
  delay(2 * SECONDS_TO_MILLIS);

  logger.debug("end");
}

void test2() {
  logger.debug("initDisplay");
  display.init();
  logger.debug("ui_init");
  ui_init();
  logger.debug("initDisplay done, entering infinite loop");
  for (int i = 0; i < 10000; i++) {
    logger.debug("in the loop, i=%d", i);
    lv_disp_load_scr(ui_OKScreen);
    lv_label_set_text(ui_LabelRemainingWattsOK, i % 2 == 0 ? "even" : "odd");
    display.loop();
    delay(2 * SECONDS_TO_MILLIS);
    lv_disp_load_scr(ui_WarningScreen);
    display.loop();
    delay(2 * SECONDS_TO_MILLIS);
  }
}

uint32_t _htmlToHexColor(const char* htmlColor) {
  if (htmlColor == nullptr) {
    logger.debug("HTML color is null");
    return 0xF000F0;
  }
  if (htmlColor[0] != '#' || strlen(htmlColor) != 7) {
    logger.debug("Invalid HTML color: %s", htmlColor);
    return 0xF000F0;
  }
  uint32_t color = (uint32_t)strtol(htmlColor + 1, nullptr, 16);
  return color;
}

void parseUIColors(JsonObject colorsJson) {
  logger.debug("Parsing UI colors from JSON config");

  // copy all from json to uiColors
  uiColors.okState.panelTop.bg = _htmlToHexColor(colorsJson["okState"]["panelTop"]["bg"]);
  uiColors.okState.panelTop.border = _htmlToHexColor(colorsJson["okState"]["panelTop"]["border"]);
  uiColors.okState.labels.text = _htmlToHexColor(colorsJson["okState"]["labels"]["text"]);
  uiColors.okState.arcWatts.main = _htmlToHexColor(colorsJson["okState"]["arcWatts"]["main"]);
  uiColors.okState.arcWatts.worstCaseIndicator = _htmlToHexColor(colorsJson["okState"]["arcWatts"]["worstCaseIndicator"]);
  uiColors.okState.arcWatts.normalIndicator = _htmlToHexColor(colorsJson["okState"]["arcWatts"]["normalIndicator"]);
  uiColors.warningState.panelTop.bg = _htmlToHexColor(colorsJson["warningState"]["panelTop"]["bg"]);
  uiColors.warningState.panelTop.border = _htmlToHexColor(colorsJson["warningState"]["panelTop"]["border"]);
  uiColors.warningState.labels.text = _htmlToHexColor(colorsJson["warningState"]["labels"]["text"]);
  uiColors.warningState.labelsApplianceNormal.text = _htmlToHexColor(colorsJson["warningState"]["labelsApplianceNormal"]["text"]);
  uiColors.warningState.labelsApplianceNormal.bg = _htmlToHexColor(colorsJson["warningState"]["labelsApplianceNormal"]["bg"]);
  uiColors.warningState.labelsApplianceNormal.border = _htmlToHexColor(colorsJson["warningState"]["labelsApplianceNormal"]["border"]);
  uiColors.warningState.labelsApplianceDimmed.text = _htmlToHexColor(colorsJson["warningState"]["labelsApplianceDimmed"]["text"]);
  uiColors.warningState.labelsApplianceDimmed.bg = _htmlToHexColor(colorsJson["warningState"]["labelsApplianceDimmed"]["bg"]);
  uiColors.warningState.labelsApplianceDimmed.border = _htmlToHexColor(colorsJson["warningState"]["labelsApplianceDimmed"]["border"]);
  uiColors.warningState.arcWatts.main = _htmlToHexColor(colorsJson["warningState"]["arcWatts"]["main"]);
  uiColors.warningState.arcWatts.worstCaseIndicator = _htmlToHexColor(colorsJson["warningState"]["arcWatts"]["worstCaseIndicator"]);
  uiColors.warningState.arcWatts.normalIndicator = _htmlToHexColor(colorsJson["warningState"]["arcWatts"]["normalIndicator"]);
}

void parseJsonConfig(String payloadString) {
  logger.debug("Parsing config...");
  DeserializationError error = deserializeJson(jsonConfig, payloadString);
  if (error) {
    logger.debug("Parsing input failed: %s", error.c_str());
    display.setLoadingScreenText("Invalid config!");
    return;
  }

  // logger.debug("Parsed configuration:");
  // String tmp = "";
  // serializeJsonPretty(jsonConfig, tmp);
  // logger.debug(tmp.c_str());

  demo.setActive(jsonConfig["settings"]["demo_mode"] | false);

  if (jsonConfig["settings"]["colors"].is<JsonObject>()) {
    parseUIColors(jsonConfig["settings"]["colors"]);
    display.applyUIColors();
  }

  mainMeter.setNominalVoltage((int)jsonConfig["electricity"]["meter"]["voltage"]);
  mainMeter.setMaxCurrent((int)jsonConfig["electricity"]["meter"]["current"]);
  display.handleElectricityMeterConfigChange(mainMeter.getMaxAllowedWatts());

  mqttTopicMainElectricityMeterPower = (const char*)jsonConfig["topics"]["current_power"];
  logger.debug("Subscribing to power meter topic [%s]", mqttTopicMainElectricityMeterPower.c_str());
  bool ok = mqttClient.subscribe(mqttTopicMainElectricityMeterPower.c_str());
  if (!ok) {
    display.setLoadingScreenText("MQTT subscription failed!");
  }

  auto appliancesJsonArray = jsonConfig["electricity"]["appliances"].as<JsonArray>();
  logger.debug("Found %d appliances in config:", appliancesJsonArray.size());

  appliances.clear();
  for (JsonObject applianceJson : appliancesJsonArray) {
    Appliance appliance = Appliance(logger);
    appliance.updateFromJson(applianceJson);
    appliances.push_back(appliance);
    logger.debug("  Appliance: %s (max power %.2f W)", appliance.nameNominative.c_str(), appliance.maxPower);

    if (appliance.jsonTopicName != "") {
      String topic = appliance.jsonTopicName;
      logger.debug("    Subscribing to individual power meter topic [%s]", topic.c_str());
      bool ok = mqttClient.subscribe(topic.c_str());
      if (!ok) {
        display.setLoadingScreenText("MQTT subscription failed!");
      }
    }
  }

  logger.debug("Configured %d appliances", appliances.size());
  if (appliances.size() > MAX_APPLIANCES) {
    // The total number of appliances is not an issue. It would be a problem only if all of them should be displayed at the same time.
    logger.debug("Too many appliances in config, only %d are supported on screen", MAX_APPLIANCES);
  }
}

void handleMainElectricityMeterUpdate(String payloadString) {
  if (demo.isActive()) {
    // in demo mode, ignore real power meter updates
    return;
  }
  mainMeter.powerReading.update(payloadString.toFloat());
}

void handleIndividualPowerMeterUpdate(Appliance& appliance, String payloadString) {
  if (demo.isActive()) {
    // in demo mode, ignore real power meter updates
    return;
  }

  // logger.debug("Handling individual power meter topic [%s]", topicString.c_str());

  // parse payload as JSON and get the value at json_field (e.g. { "power": 123.4 } and json_field = "power" -> 123.4)
  JsonDocument jsonIndividualMeterData;
  DeserializationError error = deserializeJson(jsonIndividualMeterData, payloadString);
  if (error) {
    logger.debug("Parsing individual power meter input failed: %s", error.c_str());
    return;
  }
  appliance.powerReading.update(jsonIndividualMeterData[appliance.jsonFieldName]);
  // logger.debug("Appliance [%s] power updated to %.2f W", appliance.nameNominative.c_str(), appliance.powerReading.getLast());
}

// handle message arrived
void MQTTcallback(char* topic, byte* payload, unsigned int length) {
  String topicString = String(topic);
  String payloadString = "";
  for (int i = 0; i < length; i++) {
    payloadString += (char)payload[i];
  }

  // logger.debug("Message arrived on topic [" + topicString + "] Payload: [" + payloadString + "]");
  if (topicString == MQTT_CONFIGURATION_TOPIC) {
    parseJsonConfig(payloadString);
  } else if (topicString == mqttTopicMainElectricityMeterPower) {
    handleMainElectricityMeterUpdate(payloadString);
  } else {
    // try to find individual power meter topics
    bool found = false;
    for (int i = 0; i < appliances.size(); i++) {
      Appliance& appliance = appliances[i];
      if (topicString == appliance.jsonTopicName) {
        found = true;
        handleIndividualPowerMeterUpdate(appliance, payloadString);
      }
    }
    if (!found) {
      logger.debug("Unknown topic [%s]", topicString.c_str());
    }
  }
}

void setup() {
  Serial.begin(115200); /* prepare for possible serial debug */

  display.init();
  backlight.init();
  ui_init();

  display.setLoadingScreenText("Connecting to WiFi");

  systemLayer.wdtStop();
  wifiManager.setHostname(NETWORK_HOSTNAME);
  if (!wifiManager.getWiFiIsSaved()) {
    display.setLoadingScreenText("Starting WiFi AP!");
  }
  wifiManager.autoConnect();
  systemLayer.logResetReason();
  systemLayer.wdtInit();

  display.setLoadingScreenText("Enabling OTA");
  // enable OTA
  ArduinoOTA.setHostname(NETWORK_HOSTNAME);
  ArduinoOTA.begin();
  ArduinoOTA.onStart([]() {
    logger.debug("OTA Start");

    systemLayer.wdtStop();

    backlight.setBacklight(1);
    backlight.stopTimeout();

    lv_disp_load_scr(ui_LoadingScreen);
    display.setLoadingScreenText("OTA in progress...");
    display.loop();
  });
  ArduinoOTA.onEnd([]() { logger.debug("OTA End"); });

  // Now with OTA enabled we should check if the previous reset was due to faulty software and if so then wait before continuing boot
  if (systemLayer.resetReasonWasFaultySoftware()) {
    display.setLoadingScreenText("Send OTA within 30s...");
    while (!safeModeDelayTimer.expired()) {
      ArduinoOTA.handle();
      delay(100);
    }
    logger.debug("Rebooting after faulty software reset");
    ESP.restart();
  }

  backlight.registerAfterChangeCallback([](int on_off) { homeassistant.publish_backlight(false, on_off); });

  // #ifdef USE_WDT
  //   if (esp_reset_reason() == ESP_RST_TASK_WDT) {
  //     sleepTime = SLEEP_TIME_PERMANENT_ERROR;
  //     error("Watchdog issue. Please report this to the developer.");
  //   }
  // #endif

  display.setLoadingScreenText("Connecting to MQTT");
  mqttWrapper.init(&MQTTcallback);

  display.setLoadingScreenText("Publishing HA MQTT config");
  homeassistant.publish_backlight(true, 0);
  homeassistant.publish_uptime_if_needed(true);
  homeassistant.publish_warningstate(true, 0);

  // display.setLoadingScreenText("Loading configuration");
  // while (mqttTopicMainElectricityMeterPower == "") {
  //   loop();
  // }

  display.setLoadingScreenText("Initialization done");
  displayRecalculate.start();

  logger.debug("Setup done, version %s", VERSION);
}

void loop() {
  ArduinoOTA.handle();
  mqttClient.loop();
  backlight.loop();
  mqttWrapper.loop();

  if (displayRecalculate.expired()) {
    // update the display from the accumulated power reading
    if (display.updateFromPowerReading(appliances, mainMeter)) {
      homeassistant.publish_warningstate(false, 0);
    } else {
      homeassistant.publish_warningstate(false, 1);
    }
  }

  demo.loop();
  display.loop();

  homeassistant.publish_uptime_if_needed(false);
  delay(5 * MILLIS);
}
