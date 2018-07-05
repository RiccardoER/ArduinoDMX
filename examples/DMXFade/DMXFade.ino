/*
  DMX Fade

  This sketch fades the value of slot 1 between 0 and 255 in steps to create a fade effect.
  All other slots are set to a value of 0.

  Circuit:
   - DMX light
   - MKR board
   - MKR 485 shield
     - ISO GND connected to DMX light GND (pin 1)
     - Y connected to DMX light Data + (pin 2)
     - Z connected to DMX light Data - (pin 3)
     - Jumper positions
       - Z \/\/ Y set to ON

  created 5 July 2018
  by Sandeep Mistry
*/

#include <RS485.h> // the DMX library depends on the RS485
#include <DMX.h>

const int universeSize = 16;

int brightness = 0;
int fadeAmount = 5;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // initialize the DMX library with the universe size
  if (!DMX.begin(universeSize)) {
    Serial.println("Failed to initialize DMX!");
    while (1); // wait for ever
  }
}

void loop() {
  // set the value of slot 1, unwritten slots are set to value of 0
  DMX.beginTransmission();
  DMX.write(1, brightness);
  DMX.endTransmission();

  // change the brightness for the next round
  brightness += fadeAmount;

  // reverse fade direction when on edge of range
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }

  // delay for dimming effect
  delay(30);
}

