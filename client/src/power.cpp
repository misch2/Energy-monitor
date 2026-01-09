#include "power.h"

#define SECONDS_TO_MILLIS 1000

JsonDocument config;
// parsed values from config
String mqttTopicCurrentPower = "";
int voltage = 0;
int maxCurrent = 0;
int limitWatts = 0;
JsonArray appliances = JsonArray();                                    // empty array for the start
std::array<float, MAX_APPLIANCES> applianceWorstCaseCorrection = {0};  // per-appliance correction factors to adjust worst-case power consumption

// values from meters
float currentRealWatts = 0.0;
float filteredCurrentRealWatts = 0.0;

float currentWorstCaseWatts = 0.0;
float filteredCurrentWorstCaseWatts = 0.0;

const int MAX_POWER_READINGS_BUFFER = 10;
CircularBuffer<float, MAX_POWER_READINGS_BUFFER> powerReadingsBuffer;
CircularBuffer<float, MAX_POWER_READINGS_BUFFER> worstCasePowerReadingsBuffer;

bool showApplianceLabel(lv_obj_t* ui_element, int number, int remainingWatts) {
  // logger.debug("in showApplianceLabel(%d, %d)", number, remainingWatts);

  if (number > appliances.size()) {
    // logger.debug("Appliance #%d not found in config, skipping this label", number);
    lv_obj_add_flag(ui_element, LV_OBJ_FLAG_HIDDEN);
    return false;
  }

  JsonVariant appliance = appliances[number - 1];
  if (remainingWatts > (int)appliance["power"]) {
    // logger.debug("Appliance %d compliant, remainingWatts %d > %d, skipping this label", number, remainingWatts, (int)appliance["power"]);
    lv_obj_add_flag(ui_element, LV_OBJ_FLAG_HIDDEN);
    return false;
  };

  String name = appliance["name"]["accusative"];
  // logger.debug("Appliance %d displayed, remainingWatts %d <= %d, name: %s", number, remainingWatts, (int)appliance["power"], name.c_str());
  lv_label_set_text(ui_element, name.c_str());
  lv_obj_clear_flag(ui_element, LV_OBJ_FLAG_HIDDEN);

  return true;
}

void handleMQTTMessageCurrentPower(String payloadString) {
  currentRealWatts = payloadString.toFloat();

  currentWorstCaseWatts = currentRealWatts;
  // adjust for worst-case power consumption of detected appliances
  for (size_t i = 0; i < appliances.size(); i++) {
    currentWorstCaseWatts += applianceWorstCaseCorrection[i];
  }

  // add currentWatts to buffer and find max over last MAX_POWER_READINGS_BUFFER readings
  powerReadingsBuffer.push(currentRealWatts);
  filteredCurrentRealWatts = currentRealWatts;
  for (decltype(powerReadingsBuffer)::index_t i = 0; i < powerReadingsBuffer.size(); i++) {
    if (powerReadingsBuffer[i] > filteredCurrentRealWatts) {
      filteredCurrentRealWatts = powerReadingsBuffer[i];
    }
  };

  // dtto for worst-case scenario
  worstCasePowerReadingsBuffer.push(currentWorstCaseWatts);
  filteredCurrentWorstCaseWatts = currentWorstCaseWatts;
  for (decltype(worstCasePowerReadingsBuffer)::index_t i = 0; i < worstCasePowerReadingsBuffer.size(); i++) {
    if (worstCasePowerReadingsBuffer[i] > filteredCurrentWorstCaseWatts) {
      filteredCurrentWorstCaseWatts = worstCasePowerReadingsBuffer[i];
    }
  };

  updateCurrentPower();
}

void updateCurrentPower() {
  float currentWatts = currentRealWatts;
  float filteredWatts = filteredCurrentRealWatts;

  // if (1) {  // FIXME make it configurable: use pessimistic value for both
  //   currentWatts = currentWorstCaseWatts;
  //   filteredWatts = filteredCurrentWorstCaseWatts;
  // }

  float remainingWatts = limitWatts - filteredWatts;              // time filtered and rounded down value for the warning message, to prevent flickering
  int displayedRemainingWatts = (int)(remainingWatts / 50) * 50;  // round down to multiples of 50

  // non-filtered and more precise value for the gauge
  lv_arc_set_value(ui_ArcCurrentWattsOK, currentWatts);
  lv_arc_set_value(ui_ArcCurrentWattsWarning, currentWatts);
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
    setBacklight(1);
    backlightTimeout.start();  // turn off backlight after 30 seconds
    publish_homeassistant_value_warningstate(false, 1);
  } else {
    lv_disp_load_scr(ui_OKScreen);
    publish_homeassistant_value_warningstate(false, 0);
  }
}

void handleMQTTMessageConfiguration(String payloadString) {
  logger.debug("Parsing config...");
  DeserializationError error = deserializeJson(config, payloadString);
  if (error) {
    logger.debug("Parsing input failed: %s", error.c_str());
    setLoadingScreenText("Invalid config!");
    return;
  }

  // logger.debug("Parsed configuration:");
  // String tmp = "";
  // serializeJsonPretty(config, tmp);
  // logger.debug(tmp.c_str());

  voltage = (int)config["electricity"]["meter"]["voltage"];
  maxCurrent = (int)config["electricity"]["meter"]["current"];
  limitWatts = voltage * maxCurrent;
  lv_arc_set_range(ui_ArcCurrentWattsOK, 0, limitWatts);
  lv_arc_set_range(ui_ArcCurrentWattsWarning, 0, limitWatts);
  lv_arc_set_range(ui_ArcWorstCaseWattsOK, 0, limitWatts);
  lv_arc_set_range(ui_ArcWorstCaseWattsWarning, 0, limitWatts);

  mqttTopicCurrentPower = (const char*)config["topics"]["current_power"];
  logger.debug("Subscribing to power meter topic [%s]", mqttTopicCurrentPower.c_str());
  bool ok = mqttClient.subscribe(mqttTopicCurrentPower.c_str());
  if (!ok) {
    setLoadingScreenText("MQTT subscription failed!");
  }

  appliances = config["electricity"]["appliances"].as<JsonArray>();
  logger.debug("Configured %d appliances", appliances.size());
  if (appliances.size() > MAX_APPLIANCES) {
    // logger.debug("Too many appliances in config, only %d are supported", MAX_APPLIANCES);
    // The total number of appliances is not an issue. It would be a problem only if all of them should be displayed at the same time.
  }

  applianceWorstCaseCorrection.fill(0.0);  // reset all correction factors

  // subscribe to all other topics if needed in appliances/individual_power_meter/json_topic
  for (JsonVariant appliance : appliances) {
    if (appliance["individual_power_meter"]["json_topic"]) {
      String topic = appliance["individual_power_meter"]["json_topic"];
      logger.debug("Subscribing to individual power meter topic [%s]", topic.c_str());
      bool ok = mqttClient.subscribe(topic.c_str());
      if (!ok) {
        setLoadingScreenText("MQTT subscription failed!");
      }
    }
  }
}
