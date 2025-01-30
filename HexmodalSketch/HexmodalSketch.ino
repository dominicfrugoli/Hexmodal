#include <Wire.h>
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

Adafruit_MPR121 cap = Adafruit_MPR121(); // Instaniates object for capacitive breakout
uint16_t lasttouched = 0; // For keeping track of when cap pins are touched and released
uint16_t currtouched = 0;
int sensorData[12]; // Array for keeping capacitive plate states

const int ribbonPin = A0; // Ribbon Sensor Variables
int ribbonValue = 0;

const int buttonPin = 9; // Button Variables
int buttonState = 0;
bool lastButtonState = LOW;
int buttonMode = 0;

const int switchPin = 12; // Switch Variables
int switchState = 0;


void setup() 
{
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(switchPin, INPUT_PULLUP);
  
  Serial.begin(9600);
  while (!Serial) // Start delay (from MPR121 example) 
  { 
    delay(10);
  }  
  
  // Code for making sure MPR board is found, it seems to be integral but I don't know why  
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");  
}


void loop() 
{  
  //--------------------------INPUT READINGS--------------------------//
  
  currtouched = cap.touched(); // Get the currently touched pads  
  for (uint8_t i=0; i<12; i++) // Runs through all pins
  {
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) // Runs when touched
    {
      sensorData[i] = 1;
    }
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) // Runs when released
    {
      sensorData[i] = 0;
    }
  }  
  ribbonValue = analogRead(ribbonPin); // Reads ribbon sensor
  // TODO: ADD MODE SWITCH LOGIC
  buttonState = digitalRead(buttonPin); // Reads button state
  switchState = digitalRead(switchPin); // Reads switch state
  

  //--------------------------SERIAL PRINTING--------------------------//
  
  for(int i = 0; i < 12; i++) // This loop prints the serial data in white-space separated list
  {
    Serial.print(sensorData[i]); Serial.print(" ");
  }
  Serial.print(ribbonValue); Serial.print(" "); // Adds the ribbon value to the serial message string
  Serial.print(buttonState); Serial.print(" "); // Adds button state to to the serial message string
  Serial.print(switchState); Serial.print(" "); // Adds button state to to the serial message string
  Serial.println();

  // SERIAL OUTPUT: capPlates(1-12) ribbonValue buttonState switchState

  // reset our state
  lasttouched = currtouched;

  return;
}
