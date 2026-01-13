#pragma once
#include <cstdint>

class UIColors {
 public:
  struct PanelTop {
    uint32_t bg;
    uint32_t border;
  };

  struct ArcWatts {
    uint32_t main;
    uint32_t worstCaseIndicator;
    uint32_t normalIndicator;
  };

  struct Labels {
    uint32_t text;
  };

  struct LabelsAppliance {
    uint32_t text;
    uint32_t bg;
    uint32_t border;
  };

  //   struct TextAreaAppliancesWarning {
  //     uint32_t bg;
  //   };

  struct OkState {
    PanelTop panelTop = {0x007F00, 0x7FFF7F};
    ArcWatts arcWatts = {0x404040, 0x2080FF, 0xFFFFFF};
    Labels labels = {0xFFFFFF};
  };

  struct WarningState {
    PanelTop panelTop = {0xA80000, 0xFF8080};
    ArcWatts arcWatts = {0x400000, 0x804040, 0xFFFFFF};
    Labels labels = {0xFFFFFF};
    LabelsAppliance labelsApplianceNormal = {0x000000, 0xFFFFFF, 0xFFFFFF};
    LabelsAppliance labelsApplianceDimmed = {0x000000, 0x404040, 0x404040};
    // TextAreaAppliancesWarning textAreaAppliancesWarning = {0xA80000};
  };

  OkState okState;
  WarningState warningState;
};
