# crybabyFX
"CryBaby" Prop FX - a compact, customizeable Arduino-based soundboard and effects driver.

Dependencies: Requires Adafruit's Soundboard, LED Backpack, and GFX libraries present in your Arduino IDE in order to compile.

Current Version: 1.0.1
- Stable and usable, with muzzle strobe, LED alphanumeric display, and serial audio control all functional.

CHANGELOG:
- 1.0.1: 
   - Removed all code referring to the GPIO audio pins ("fireSoundPin", etc.), these are no longer needed due to serial audio control.
   - Added "altModePin1" and "altModePin2" pins for the left and right positions on the mode selector switch, necessary for Music Support feature planned for v1.1.  They are assigned to pins 12 and 13 respectively.
   - Various other cleanup.

PLANNED CHANGES:
For next build (1.1):
- Add music support.  There are some currently unused switch positions on the front control grip that I would like to use for audio playback.  Since the audio board is running in UART Serial mode for faster response, this will need to be done via software logic.
- Remove all uses of delay() function.  For smoother operation, I want to replace all delay() calls with millis() timer loops.  Verified Pro Trinket board is capable of millis() timer loops, so need to figure out what I have been doing wrong/why they aren't working.
- Re-tweak audio. Reminder: Firing "Tail" sound is currently disabled.  This will be re-enabled as part of the audio loop cleanup.

For 1.2: 
- Create a Pulse Rifle branch of the code

Future:
- Create a "Barebones" branch supporting MOSFET LED control, LED alphanumeric display, serial audio control, and non-blocking timer loops, intended for customization by the end user to meet the needs of a specific prop.
