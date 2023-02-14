// 1- Yuanxu Zhang - yzhan319 and Jack Li - wli200
// 2- Project - Ikun Joystick game
// 3- Description - Two player use joystick to play a game and answer a series of question to seehow much they know about each other.
// 4- Assumptions - Each play have one joystick to control the game.After the game they will have a score of how much they know about each other.
// Include Libraries
#include "Arduino.h"
#include "Joystick.h"


// Pin Definitions
#define JOYSTICK_PIN_SW1  4
#define JOYSTICK_PIN_SW2  6
#define JOYSTICK_PIN_VRX1 A1
#define JOYSTICK_PIN_VRX2 A3
#define JOYSTICK_PIN_VRY1 A2
#define JOYSTICK_PIN_VRY2 A4
#define BSTART 10
#define BRESET 8
#define BUZZER 5

int startState = LOW;
int resetState = LOW;
int startHold = 0;
int resetHold = 0;

Joystick joystick1(JOYSTICK_PIN_VRX1,JOYSTICK_PIN_VRY1,JOYSTICK_PIN_SW1);
Joystick joystick2(JOYSTICK_PIN_VRX2,JOYSTICK_PIN_VRY2,JOYSTICK_PIN_SW2);


// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{

    Serial.begin(9600);

    pinMode(JOYSTICK_PIN_VRX1, INPUT);
    pinMode(JOYSTICK_PIN_VRX2, INPUT);
    pinMode(BSTART, INPUT);
    pinMode(BRESET, INPUT);
    pinMode(BUZZER, OUTPUT);
    
}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop() 
{
    int joystickX1 =  joystick1.getX();
    int joystickX2 =  joystick2.getX();
    int joystickY1 =  joystick1.getY();
    int joystickY2 =  joystick2.getY();
    
    startState = digitalRead(BSTART);
    resetState = digitalRead(BRESET);

    if (startState == HIGH) {
      if (startHold == 0) {
        Serial.write('s');
      }
      startHold = 1;
    } else {
      startHold = 0;
    }

    if (resetState == HIGH) {
      if (resetHold == 0) {
        Serial.write('a');
      }
      resetHold = 1;
    } else {
      resetHold = 0;
    }

    if (joystickX1 < 400) {
      Serial.write('L');
      delay(300);
    } else if (joystickX1 > 600) {
      Serial.write('R');
      delay(300);
    }

    if (joystickX2 < 400) {
      Serial.write('l');
      delay(300);
    } else if (joystickX2 > 600) {
      Serial.write('r');
      delay(300);
    }

    if (joystickY1 > 600) {
      Serial.write('1');
      delay(1000);
    }

    if (joystickY2 > 600) {
      Serial.write('2');
      delay(1000);
    }

    if (Serial.available() > 0) {
      char input = Serial.read();
      if (input == 'n') {
        tone(BUZZER, 255);
        digitalWrite(BUZZER, HIGH);
        delay(500);
        noTone(BUZZER);
      } else if (input == 'm') {
        tone(BUZZER, 0);
        digitalWrite(BUZZER, HIGH);
        delay(500);
        noTone(BUZZER);
      }
    }

    

    

    
    
}
