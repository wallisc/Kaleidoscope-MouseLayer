// Kaleidoscope-MouseLayer - A MouseLayer plugin for Kaleidoscope.

#include "MouseLayer.h"
#include "MouseKeys/MouseKeyDefs.h"
#include "kaleidoscope/plugin/LEDControl/LEDUtils.h"
#include "kaleidoscope.h"
#include "kaleidoscope/driver/hid/base/Keyboard.h"

namespace kaleidoscope {
namespace plugin {

bool MouseLayer::MouseLayerActive = false;
bool MouseLayer::MouseLayerLEDsActive = false;
uint16_t MouseLayer::ShutOffTimer = 0;
uint16_t MouseLayer::RainbowHue = 0;
bool MouseLayer::ShutOffCountdownStarted = false;

#define UPDATE_LED_STATE_EVERY_CYCLE 1

const kaleidoscope::Key LeftClickKeys[] = { Key_K, Key_D };
const kaleidoscope::Key MiddleClickKeys[] = { Key_L, Key_S };
const kaleidoscope::Key RightClickKeys[] = { Key_Semicolon, Key_A };
const kaleidoscope::Key BackClickKeys[] = { Key_I, Key_E };
const kaleidoscope::Key ForwardClickKeys[] = { Key_H, Key_G };

bool IsLeftClickKey(const Key& key) {
	for (auto& LeftClickKey : LeftClickKeys) {
		if (key == LeftClickKey) return true;
	}
	return false;
}

bool IsRightClickKey(const Key& key) {
	for (auto& RightClickKey : RightClickKeys) {
		if (key == RightClickKey) return true;
	}
	return false;
}

bool IsMiddleClickKey(const Key& key) {
	for (auto& MiddleClickKey : MiddleClickKeys) {
		if(key == MiddleClickKey) return true;
	}
	return false;
}

bool IsForwardClickKey(const Key& key) {
	for (auto& ForwardClickKey : ForwardClickKeys) {
		if (key == ForwardClickKey) return true;
	}
	return false;
}

bool IsBackClickKey(const Key& key) {
	for (auto& BackClickKey : BackClickKeys) {
		if (key == BackClickKey) return true;
	}
	return false;
}

bool IsMouseKey(const Key& key) {
	return IsLeftClickKey(key) || IsMiddleClickKey(key) || IsRightClickKey(key) || 
		IsBackClickKey(key) || IsForwardClickKey(key);
}

EventHandlerResult MouseLayer::onKeyswitchEvent(Key& mapped_key, KeyAddr key_addr, uint8_t keyState)
{
	bool bMouseButtonPressed = false;
	if (MouseLayerActive)
	{
		if (IsLeftClickKey(mapped_key)) {
			mapped_key = Key_mouseBtnL;
			bMouseButtonPressed = true;
		}
		else if (IsRightClickKey(mapped_key)) {
			mapped_key = Key_mouseBtnR;
			bMouseButtonPressed = true;
		}
		else if (IsMiddleClickKey(mapped_key)) {
			mapped_key = Key_mouseBtnM;
			bMouseButtonPressed = true;
		}
		else if (IsBackClickKey(mapped_key))
		{
			mapped_key = Key_mouseBtnP;
		}
		else if (IsForwardClickKey(mapped_key))
		{
			mapped_key = Key_mouseBtnN;
		}
	}

	// This ensures that if your mouse is standing still but you're holding down a mouse 
	// button, the mouse layer won't shut off. This is useful for things like drag and drop
	// where you might pause and don't want the mouse layer suddenly to poof
	if (bMouseButtonPressed)
	{
		ShutOffCountdownStarted = false;
	}

	return EventHandlerResult::OK;
}

EventHandlerResult MouseLayer::afterEachCycle() {
	if (Runtime.hasTimeExpired(RainbowHueUpdateTimer, RainbowHueDelay)) {
		RainbowHueUpdateTimer += RainbowHueDelay;
		RainbowHue += 1;
		if (RainbowHue >= 255) {
			RainbowHue = 0;
		}
	}

	bool bIsNumLockEnabled = !!(Kaleidoscope.hid().keyboard().getKeyboardLEDs() & LED_NUM_LOCK);

	if (bIsNumLockEnabled)
	{
		MouseLayerActive = true;
		ShutOffCountdownStarted = false;
	}

	if (MouseLayerActive && !bIsNumLockEnabled)
	{
		if (!ShutOffCountdownStarted)
		{
			ShutOffCountdownStarted = true;
			ShutOffTimer = Runtime.millisAtCycleStart();
		}
		else if (Runtime.hasTimeExpired(ShutOffTimer, ShutoffDelay))
		{
			MouseLayerActive = false;
		}
	}
	
	if (MouseLayerActive) {
#if !UPDATE_LED_STATE_EVERY_CYCLE
		if (!MouseLayerLEDsActive) {
			::LEDControl.set_mode(::LEDControl.get_mode_index());
#endif
			const cRGB mouseLayerColor = hsvToRgb(RainbowHue, 255, 255);

			for (auto key_addr : KeyAddr::all()) {
				Key k = Layer.lookupOnActiveLayer(key_addr);

				if (IsMouseKey(k)) {
					::LEDControl.setCrgbAt(KeyAddr(key_addr), mouseLayerColor);
				}
			}
			MouseLayerLEDsActive = true;
#if !UPDATE_LED_STATE_EVERY_CYCLE
		}
#endif
	}
	else if (MouseLayerLEDsActive)
	{
		::LEDControl.set_mode(::LEDControl.get_mode_index());
		MouseLayerLEDsActive = false;
	}
	return EventHandlerResult::OK;
}


}
}

kaleidoscope::plugin::MouseLayer MouseLayer;
