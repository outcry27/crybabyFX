# crybabyFX
"CryBaby" Prop FX - a compact, customizeable Arduino-based soundboard and effects driver.

Dependencies: Requires Adafruit's Soundboard, LED Backpack, and GFX libraries present in your Arduino IDE in order to compile.


NOTE ON CUSTOMIZING THE AUDIO: 
Triggering the audio files via UART Serial command works by calling them by their relative number in a list, rather than by their filename: "Serial.print("#4\n")" or "Serial.print("#0\n")", for example.  However... for whatever reason, the Audio FX board lists the files in *REVERSE* order.  In other words, if you have "T00.wav" through "T05.wav" stored on the FX board, it will list T05.wav as File 0 and T00.wav as File 6.)  So, it's important to remember that any time you add or remove audio files, you either have to update the serial calls in the code, or you have t0 change all of the file names so their relative positions in the list remain the same.


Current Version: 1.0.3
- Stable and usable, with muzzle strobe, LED alphanumeric display, and serial audio control all functional.
- Support implemented for the slider switch positions.  Two music clips uploaded to the Audio Board (.OGG format), CryBaby software logic updated accordingly.


CHANGELOG:
- 1.0.3
   - Completed implementation of mode-select slider switch.  There is now a function for each switch position (excluding Center) which   is triggered while that pin is held LOW, and ended once the pin goes HIGH again.
      - Left position is bound to pin 12 on the Pro Trinket and "altMode1State" in the code.
      - Right position is bound to pin 13 on the Pro Trinket and "altMode2State" in the code.
      - Center position is neutral.
   - Added two new .OGG format music clips to the Audio FX board for the mode select switches to trigger.  Updated code accordingly to   ensure functions are still calling the correct audio files.
   - Further adjusted effects and "boot-up sequence" timing.
   - Overall code cleanup.

- 1.0.2: 
   - Began implementing support for mode-select slider switch.
   - Adjusted effects timing.
   
- 1.0.1: 
   - Removed all code referring to the GPIO audio pins ("fireSoundPin", etc.), these are no longer needed due to serial audio control.
   - Added "altModePin1" and "altModePin2" pins for the left and right positions on the mode selector switch, necessary for Music Support feature planned for v1.1.  They are assigned to pins 12 and 13 respectively.
   - Various other cleanup.


PLANNED CHANGES:
For next major build (1.1):
- Remove all uses of delay() function.  For smoother operation, I want to replace all delay() calls with millis() timer loops.  Verified Pro Trinket board is capable of millis() timer loops, so need to figure out what I have been doing wrong/why they aren't working.
- Re-tweak audio. Reminder: Firing "Tail" sound is currently disabled.  This will be re-enabled as part of the audio loop cleanup.

For 1.2: 
- Create a Pulse Rifle branch of the code

Future:
- Create a "Barebones" branch supporting MOSFET LED control, LED alphanumeric display, serial audio control, and non-blocking timer loops, intended for customization by the end user to meet the needs of a specific prop.
