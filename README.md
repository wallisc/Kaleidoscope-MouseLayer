# Kaleidoscope-MouseLayer
 
This plugin is for enabling certain keys to be overriden to mouse keys when mouse movement is detected.

# Installation
 ## Installation
 1. Find the plugin directory for the Kaleidoscope source. For the Dygma branch of Kaleidoscope it looks like: Arduino\hardware\dygma\samd\libraries\Kaleidoscope\src\kaleidoscope\plugin
 2. Copy the contents of the Src directory into the plugin folder using either git clone or just using the "Clone or download" button from GitHub
 3. Add the plugin to your sketch by adding the include and initializing the plugin. This plugin also requires the MouseKeys plugin and must be placed before the MouseKeys plugin in KALEIDOSCOPE_INIT_PLUGINS
 Okay firmware side of things is done. Now we need to run the script that will let the firmware know the mouse is moving
 4. Install AHK: https://www.autohotkey.com/
 5. Run the script in Script/MouseLayer.ahk
```
#include "kaleidoscope/plugin/MouseLayer.h"

KALEIDOSCOPE_INIT_PLUGINS(
	MouseKeys,
	MouseLayer);