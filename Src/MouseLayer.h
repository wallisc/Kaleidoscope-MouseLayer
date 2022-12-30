// Kaleidoscope-MouseLayer - A MouseLayer plugin for Kaleidoscope.

#pragma once

#include "Kaleidoscope-LEDControl.h"

namespace kaleidoscope {
namespace plugin {

class MouseLayer : public kaleidoscope::Plugin {
 public:
  MouseLayer(void) {}

  EventHandlerResult onKeyswitchEvent(Key& mapped_key, KeyAddr key_addr, uint8_t keyState);
  EventHandlerResult afterEachCycle();
 private:
  uint16_t RainbowHueUpdateTimer = 0;
  static constexpr uint16_t RainbowHueDelay = 25;
  static uint16_t RainbowHue;
  static constexpr uint16_t ShutoffDelay = 100;
  static uint16_t ShutOffTimer;
  static bool ShutOffCountdownStarted;
  static bool MouseLayerActive;
  static bool MouseLayerLEDsActive;
};
}
}

extern kaleidoscope::plugin::MouseLayer MouseLayer;