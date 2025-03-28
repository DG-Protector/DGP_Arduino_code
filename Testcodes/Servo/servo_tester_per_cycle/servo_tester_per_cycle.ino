/*
 * servo tester using line tracing sensor
 *
 * written by jihoonkimtech
 *
 * last modified at 2025-03-18
*/

#include <Servo.h>    // library of servo
Servo SERVO;          // object of Servo class

#define SERVO_PIN 10  // servo motor
#define SENSOR 7      // line tracing sensor
#define BKWD_BUT 3    // backward button
#define FRWD_BUT 2    // forward button

#define SERVO_FRWD 110  // rotate clockwise servo
#define SERVO_BKWD 70   // rotate anti-clockwise servo
#define SERVO_STOP 90   // stop rotate servo

void setup() {
  Serial.begin(9600);               // for debug

  pinMode(BKWD_BUT, INPUT_PULLUP);
  pinMode(FRWD_BUT, INPUT_PULLUP);
  pinMode(SENSOR, INPUT);           // pinmode set-up

  SERVO.attach(SERVO_PIN);          // attach servo
  SERVO.write(SERVO_STOP);          // stop servo
  servo_init();
}

void loop() {
  if(_checkButton(BKWD_BUT)){      // if pressed backward button?
    servo_cycle(SERVO_FRWD);
  }

  if(_checkButton(FRWD_BUT)){      // if press forward button?
    servo_cycle(SERVO_BKWD);
  }
}

bool _checkButton(int pin){        // double check for chattering
  if(!digitalRead(pin)){
    delay(50);
    if(!digitalRead(pin)) return true;
    else return false;
  } else {
    return false;
  }
}

bool _checkSensor(int pin){       // inspect sensor values for instability
  bool prev = digitalRead(pin);   // check value
  delay(50);
  bool now = digitalRead(pin);
  delay(50);
  
  if(prev != now)                 // not equal prev value and new value?
    bool now = digitalRead(pin);  // re-sensing
  
  return now;
}

void servo_init() {                   // initializing servo motor
  if(_checkSensor(SENSOR)) return;    // already reached the point of reference -> end.


  SERVO.write(SERVO_BKWD);            // servo rotate
  while(!_checkSensor(SENSOR));       // break if sensor find point of reference
  SERVO.write(SERVO_STOP);            // stop servo

  return;
}

void servo_cycle(int direction) {   // rotating the servo motor around once
  servo_init();

  for(int i = 0; i < 4; i++) {
    SERVO.write(direction);
    while(_checkSensor(SENSOR));    // rotation until out of point of reference
    SERVO.write(SERVO_STOP);

    SERVO.write(direction);
    while(!_checkSensor(SENSOR));   // rotation until entering the point of reference
    SERVO.write(SERVO_STOP);
  }

  return;
}