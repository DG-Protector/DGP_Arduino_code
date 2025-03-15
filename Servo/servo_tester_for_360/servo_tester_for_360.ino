/*
 * servo tester for 360
 *
 * written by jihoonkimtech
 *
 * last modified at 2025-02-26
*/

#include <Servo.h>    // library of servo
Servo L_SERVO;          // object of Servo class
Servo R_SERVO;          // object of Servo class

#define L_SERVO_PIN 10  // servo motor
#define R_SERVO_PIN 11  // servo motor
#define BKWD_BUT 3    // backward button
#define FRWD_BUT 2    // forward button

#define SERVO_FRWD 100  // rotate clockwise servo
#define SERVO_BKWD 80   // rotate anti-clockwise servo
#define SERVO_STOP 90   // stop rotate servo

void setup() {
  Serial.begin(9600);       // for debug

  pinMode(BKWD_BUT, INPUT_PULLUP);
  pinMode(FRWD_BUT, INPUT_PULLUP);

  L_SERVO.attach(L_SERVO_PIN);  // attach servo
  R_SERVO.attach(R_SERVO_PIN);  // attach servo
  L_SERVO.write(SERVO_STOP);  // stop servo
  R_SERVO.write(SERVO_STOP);  // stop servo
}

void loop() {
  if(_checkButton(BKWD_BUT)){      // if pressed backward button?
    while(_checkButton(BKWD_BUT)){ // rotating backward while press
      L_SERVO.write(SERVO_BKWD);
      R_SERVO.write(SERVO_BKWD);
      delay(50);
    }
  }

  if(_checkButton(FRWD_BUT)){      // if press forward button?
    while(_checkButton(FRWD_BUT)){ // rotating forward while press
      L_SERVO.write(SERVO_FRWD);
      R_SERVO.write(SERVO_FRWD);
      delay(50);
    }
  }

  L_SERVO.write(SERVO_STOP);  // stop servo
  R_SERVO.write(SERVO_STOP);  // stop servo
}

bool _checkButton(int pin){ // double check for chattering
  if(!digitalRead(pin)){
    delay(50);
    if(!digitalRead(pin)) return true;
    else return false;
  } else {
    return false;
  }
}