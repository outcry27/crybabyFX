/*
  CryBaby FX Kit v1.0, written by Sean Maio (outcry27@gmail.com)
  Designed for use with the Adafruit Pro Trinket and Adafruit audio FX board

  Credit and many thanks to the writers of the numerous tutorials available on both the Arduino
  and Adafruit websites, without which this project could never have been completed!

  -Pin Map-
  Pro Trinket:
  0. Serial RX, connect to Audio FX Serial TX
  1. Serial TX, connect to Audio FX Serial RX
  3. Fire Switch -> GND
  4. Safety toggle switch -> GND
  9. Muzzle LED, connect this to the GATE pin on the muzzle strobe MOSFET
  12. Alternate Mode 1 (mode select switch left)
  13. Alternate Mode 2 (mode select switch right)
  A4. SDA (Serial Data to LED counter Pin 2)
  A5. SCL (Serial Clock to LED counter Pin 1)

  Audio FX board:
  TX. Connect to Trinket Pin 2 (labeled RX)
  RX. Connect to Trinket Pin 1 (labeled TX)
  UG. Jumper to GND to enable UART mode.

*/

#include <Wire.h>
#include <SoftwareSerial.h>
#include "Adafruit_Soundboard.h"
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

// pins used for Serial communication with the audio board
#define SFX_TX 1
#define SFX_RX 0

SoftwareSerial ss = SoftwareSerial(SFX_TX, SFX_RX);
Adafruit_Soundboard sfx = Adafruit_Soundboard(&ss, NULL, NULL);
Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

// constants:
const int triggerPin = 3;     // the number of the firing trigger pin
const int safetyPin = 4;      // pin for the "safety" toggle
const int muzzlePin =  9;     // the number of the LED pin
const int altMode1Pin = 12;   // pin for Selector Switch Left
const int altMode2Pin = 13;   // pin for Selector Switch Right (sel. switch center is default)
const int fireDelay = 50;     // duration in milliseconds for each "shot"

// variables:
boolean safetyOn = false;
boolean altMode1On = false;
boolean altMode2On = false;
int triggerState = 0;         // variable for reading the trigger status
int safetyState = 0;          // variable for reading the safety switch status
int altMode1State = 0;        // variable for reading Alt Mode 1
int altMode2State = 0;        // variable for reading Alt Mode 2
int lastTriggerState = 0;     // previous state of the trigger
int lastSafetyState = 0;      // previous state of the safety switch
int lastAltMode1State = 0;    // previous state of Alt Mode 1
int lastAltMode2State = 0;    // previous state of Alt Mode 2
int ammoCount = 299;
String ammoDisp = String(ammoCount);

void setup() {
  // initialize the LED pin as an output:
  pinMode(muzzlePin, OUTPUT);
  // initialize the trigger and safety pins as input:
  pinMode(triggerPin, INPUT);
  pinMode(safetyPin, INPUT);
  pinMode(altMode1Pin, INPUT);
  pinMode(altMode2Pin, INPUT);

  // set up beginning pin states
  digitalWrite(muzzlePin, LOW);
  digitalWrite(triggerPin, HIGH);
  digitalWrite(safetyPin, HIGH);
  digitalWrite(altMode1Pin, HIGH);
  digitalWrite(altMode2Pin, HIGH);

  Serial.begin(9600);
  ss.begin(9600);
  alpha4.begin(0x70);  // pass in the address

  delay(200);  //give the audio board time to power up; otherwise bootup sound will be called before audio board is ready to play it

  Serial.print("#0\n");  //play "boot-up" sound

  alpha4.writeDigitAscii(0, '0');
  alpha4.writeDigitAscii(1, '2');
  alpha4.writeDigitAscii(2, '9');
  alpha4.writeDigitAscii(3, '9');
  alpha4.writeDisplay();
  delay(500);

  for (int x = 4; x > 0; x--) {
    updateAmmoCounter();
    ammoCount--;
    delay(50);
  }
  updateAmmoCounter();
  delay(1100);
}

// this function calls the current value of ammoCount and writes it to the LED display
void updateAmmoCounter() {
  String ammoDisp = String(ammoCount);
  if (ammoCount < 10) {
    alpha4.writeDigitAscii(0, '0');
    alpha4.writeDigitAscii(1, '0');
    alpha4.writeDigitAscii(2, '0');
    alpha4.writeDigitAscii(3, ammoDisp[0]);
  }
  else if (ammoCount < 100) {
    alpha4.writeDigitAscii(0, '0');
    alpha4.writeDigitAscii(1, '0');
    alpha4.writeDigitAscii(2, ammoDisp[0]);
    alpha4.writeDigitAscii(3, ammoDisp[1]);
  }
  else if (ammoCount < 1000) {
    alpha4.writeDigitAscii(0, '0');
    alpha4.writeDigitAscii(1, ammoDisp[0]);
    alpha4.writeDigitAscii(2, ammoDisp[1]);
    alpha4.writeDigitAscii(3, ammoDisp[2]);
  }
  else if (ammoCount < 10000) {
    alpha4.writeDigitAscii(0, ammoDisp[0]);
    alpha4.writeDigitAscii(1, ammoDisp[1]);
    alpha4.writeDigitAscii(1, ammoDisp[2]);
    alpha4.writeDigitAscii(1, ammoDisp[3]);
  }
  alpha4.writeDisplay();
}


void setSafe() {
  safetyOn = true;
  alpha4.clear();
  alpha4.writeDisplay();
  delay(50);
  alpha4.writeDigitAscii(0, 'S');
  alpha4.writeDigitAscii(1, 'A');
  alpha4.writeDigitAscii(2, 'F');
  alpha4.writeDigitAscii(3, 'E');
  alpha4.writeDisplay();
  delay(50);
  alpha4.clear();
  alpha4.writeDisplay();
  delay(50);
  alpha4.writeDigitAscii(0, 'S');
  alpha4.writeDigitAscii(1, 'A');
  alpha4.writeDigitAscii(2, 'F');
  alpha4.writeDigitAscii(3, 'E');
  alpha4.writeDisplay();
}

void setArm() {
  safetyOn = false;

  Serial.print("#4\n");             // play "gun cocking" sound

  alpha4.writeDigitAscii(0, ' ');
  alpha4.writeDigitAscii(1, 'A');
  alpha4.writeDigitAscii(2, 'R');
  alpha4.writeDigitAscii(3, 'M');
  alpha4.writeDisplay();
  delay(50);
  alpha4.clear();
  alpha4.writeDisplay();
  delay(50);
  alpha4.writeDigitAscii(0, ' ');
  alpha4.writeDigitAscii(1, 'A');
  alpha4.writeDigitAscii(2, 'R');
  alpha4.writeDigitAscii(3, 'M');
  alpha4.writeDisplay();
  delay(50);
  alpha4.clear();
  alpha4.writeDisplay();
  delay(50);
  alpha4.writeDigitAscii(0, ' ');
  alpha4.writeDigitAscii(1, 'A');
  alpha4.writeDigitAscii(2, 'R');
  alpha4.writeDigitAscii(3, 'M');
  alpha4.writeDisplay();
  delay(50);
  alpha4.clear();
  alpha4.writeDisplay();
  delay(50);
  alpha4.writeDigitAscii(0, ' ');
  alpha4.writeDigitAscii(1, 'A');
  alpha4.writeDigitAscii(2, 'R');
  alpha4.writeDigitAscii(3, 'M');
  alpha4.writeDisplay();

  delay(300);
  updateAmmoCounter();
}

void setAltMode1()  {
  altMode1On = true;
  Serial.print("#5\n");               // play music clip 1
}

void stopAltMode1() {
  Serial.print("q/n");                  // stop sound
  altMode1On = false;
}

void setAltMode2()  {
  altMode2On = true;
  Serial.print("#6\n");               // play music clip 2
}

void stopAltMode2() {
  Serial.print("q/n");                  // stop sound
  altMode2On = false;
}

//this needs to be fixed... commented out for now
/*void playSound(int s);
  void playSound(char *name) {
  pinMode(s, OUTPUT);
  delay(100);
  pinMode(s, INPUT);
  //Serial.print("P"name"\n");
  }*/

// --MAIN LOOP STARTS HERE--
void loop() {

  safetyState = digitalRead(safetyPin);
  if (safetyState != lastSafetyState) {
    if (safetyState == HIGH) {
      if (safetyOn == false) {
        setSafe();
      }
    }
    if (safetyState == LOW) {
      if (safetyOn == true)  {
        setArm();
      }
    }
    lastSafetyState = safetyState;
  }

  altMode1State = digitalRead(altMode1Pin);
  if (altMode1State != lastAltMode1State) {
    if (altMode1State == LOW) {
      if (altMode1On == false) {
        setAltMode1();
      }
    }
    if (altMode1State == HIGH) {
      if (altMode1On == true) {
        stopAltMode1();
      }
    }
    lastAltMode1State = altMode1State;
  }

  altMode2State = digitalRead(altMode2Pin);
  if (altMode2State != lastAltMode2State) {
    if (altMode2State == LOW) {
      if (altMode2On == false)  {
        setAltMode2();
      }
    }
    if (altMode2State == LOW) {
      if (altMode2On == true) {
        stopAltMode2();
      }
    }
    lastAltMode2State = altMode2State;
  }

  triggerState = digitalRead(triggerPin);
  if (triggerState != lastTriggerState) {
    if (triggerState == HIGH) {
      // turn muzzle strobe off:
      digitalWrite(muzzlePin, LOW);
    }

    while (triggerState == LOW) {
      if (safetyOn == true) {
        Serial.print("#2\n");                   // play dry-fire "click"
        triggerState = digitalRead(triggerPin);
        return;
      }
      if (ammoCount <= 0) {
        triggerState = digitalRead(triggerPin);
        return;
      }
      if (safetyOn == false) {
        if (ammoCount > 0) {
          Serial.print("#3\n");           // start firing sound
          digitalWrite(muzzlePin, HIGH);  // turn muzzle strobe on
          ammoCount--;

          // update the ammo counter
          updateAmmoCounter();

          // cleanup (TAIL SOUND IS CURRENTLY COMMENTED OUT)
          delay(10);
          digitalWrite(muzzlePin, LOW);
          delay(39);
          triggerState = digitalRead(triggerPin);
          if (triggerState == HIGH) {
            digitalWrite(muzzlePin, LOW); // turn muzzle strobe off
            Serial.print("q\n");          // stop firing sound
            //delay(10);
            //playSound(fireTailSoundPin);
            //Serial.print("#1\n");
            //delay(400);
          }
          if (ammoCount == 0) {
            digitalWrite(muzzlePin, LOW);  // turn muzzle strobe off
            Serial.print("q\n");           // stop firing sound
            delay(10);
            Serial.print("#2\n");          // play dry-fire "click"
          }
        }
      }

      else {
        return;
      }
    }
    lastTriggerState = triggerState;

  }
}
