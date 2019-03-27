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
  
  Audio FX board:
  TX. Connect to Trinket Pin 2 (labeled RX)
  RX. Connect to Trinket Pin 1 (labeled TX)
  UG. Jumper to GND to enable UART mode. 
  
  !-- DEPRECATED: the following have been replaced by Serial triggering --!
  0. Firing sound
  1. Firing "tail"
  2. "Click" sound (out of ammo/safety on)
  3. "Boot-up" sound
  4. Music clip 1 (wired to a physical switch)
  5. Music clip 2 (wired to a physical switch)
  
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
const int safetyPin = 4; // pin for the "safety" toggle
const int fireSoundPin = 6; // pin for the firing sound
const int muzzlePin =  9;      // the number of the LED pin
const int startSoundPin = 10; // pin for the "boot-up" sound
const int clickSoundPin = 11; // pin for the bolt "click" sound
const int fireTailSoundPin = 12; // pin for the firing "tail" sound
const int fireDelay = 50; // duration in milliseconds for each "shot"

// variables:
boolean safetyOn = false;
int triggerState = 0;         // variable for reading the trigger status
int safetyState = 0;          // variable for reading the safety switch status
int lastTriggerState = 0;     // previous state of the trigger
int lastSafetyState = 0;      // previous state of the safety switch
int ammoCount = 299;
String ammoDisp = String(ammoCount);

void setup() {
  // initialize the LED pin as an output:
  pinMode(muzzlePin, OUTPUT);
  // initialize the trigger and safety pins as input:
  pinMode(triggerPin, INPUT);
  pinMode(safetyPin, INPUT);
  // initialize the audio pins
  pinMode(startSoundPin, INPUT);
  pinMode(clickSoundPin, INPUT);
  pinMode(fireTailSoundPin, INPUT);
  pinMode(fireSoundPin, INPUT);
  pinMode(triggerPin, INPUT);
  pinMode(safetyPin, INPUT);

  // set up the audio trigger pins to give a path to GND when set to OUTPUT
  digitalWrite(muzzlePin, LOW);
  digitalWrite(triggerPin, HIGH);
  digitalWrite(safetyPin, HIGH);
  digitalWrite(startSoundPin, LOW);
  digitalWrite(clickSoundPin, LOW);
  digitalWrite(fireTailSoundPin, LOW);
  digitalWrite(fireSoundPin, LOW);

  Serial.begin(9600);
  ss.begin(9600);
  alpha4.begin(0x70);  // pass in the address

  delay(200);  //give the audio board time to power up; otherwise bootup sound will be called before audio board is ready

  Serial.print("#0\n");

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
            alpha4.writeDigitAscii(2, ammoDisp[2]);
            alpha4.writeDigitAscii(3, ammoDisp[3]);
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
    if (safetyState == LOW) {
      if (safetyOn == false) {
        setSafe();
      }
      else if (safetyOn == true) {
        setArm();        
      }
    }
  }
  triggerState = digitalRead(triggerPin);
  if (triggerState != lastTriggerState) {
    if (triggerState == HIGH) {
      // turn muzzle strobe off:
      digitalWrite(muzzlePin, LOW);
    }
    
    while (triggerState == LOW) {
    if (safetyOn == true) {
      //playSound(clickSoundPin);
      Serial.print("#2\n");
      triggerState = digitalRead(triggerPin);
      return;
    }
    if (ammoCount <= 0) {
      //playSound(clickSoundPin);
      triggerState = digitalRead(triggerPin);
      return;
    }
    if (safetyOn == false) {
      if (ammoCount > 0) {
        // turn muzzle strobe on:
        //pinMode(fireSoundPin, OUTPUT);
        Serial.print("#3\n");
        digitalWrite(muzzlePin, HIGH);
        ammoCount--;
        
        // update the ammo counter
        updateAmmoCounter();
          
        // cleanup (TAIL SOUND IS CURRENTLY COMMENTED OUT)
        delay(10);
        digitalWrite(muzzlePin, LOW);
        delay(39);
        triggerState = digitalRead(triggerPin);
        if (triggerState == HIGH) {
          // turn muzzle strobe off:
          digitalWrite(muzzlePin, LOW);
          //pinMode(fireSoundPin, INPUT);
          Serial.print("q\n");
          //delay(10);
          //playSound(fireTailSoundPin);
          //Serial.print("#1\n");
          //delay(400);
        }
        if (ammoCount == 0){
          digitalWrite(muzzlePin, LOW);
          pinMode(fireSoundPin, INPUT);
          //playSound(clickSoundPin);
          Serial.print("q\n");
          delay(10);
          Serial.print("#2\n"); 
        }
      }
    }
    else {
      return;
    }
    }
  lastTriggerState = triggerState;
  }
  lastSafetyState = safetyState;  
}

