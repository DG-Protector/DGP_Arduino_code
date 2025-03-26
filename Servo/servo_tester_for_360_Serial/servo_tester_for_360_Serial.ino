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

// L

const byte R_SERVO_FRWD = 180;  // rotate clockwise servo
const byte R_SERVO_BKWD = 0;   // rotate anti-clockwise servo
const byte L_SERVO_FRWD = R_SERVO_FRWD;  // rotate clockwise servo
const byte L_SERVO_BKWD = R_SERVO_BKWD;   // rotate anti-clockwise servo

const byte SERVO_STOP = 90;   // stop rotate servo

void setup() {
  Serial.begin(115200);       // for debug

  L_SERVO.attach(L_SERVO_PIN);  // attach servo
  R_SERVO.attach(R_SERVO_PIN);  // attach servo
  L_SERVO.write(SERVO_STOP);  // stop servo
  R_SERVO.write(SERVO_STOP);  // stop servo
}

void loop() {
  if(Serial.available()){
    char buf = Serial.read();
    
    Serial.println(buf);
    if(buf == 'l'){
      L_SERVO.write(L_SERVO_BKWD);
    }
    else if(buf == 'L'){
      L_SERVO.write(L_SERVO_FRWD);
    }
    else if(buf == 'r'){
      R_SERVO.write(R_SERVO_FRWD);
    }
    else if(buf == 'R'){
      R_SERVO.write(R_SERVO_BKWD);
    }
    else if(buf == '+'){
      delay(1000);
      L_SERVO.write(L_SERVO_BKWD);
      R_SERVO.write(R_SERVO_FRWD);
    }
    else if(buf == '-'){
      delay(1000);
      L_SERVO.write(L_SERVO_FRWD);
      R_SERVO.write(R_SERVO_BKWD);
    }
    else{
      L_SERVO.write(SERVO_STOP);  // stop servo
      R_SERVO.write(SERVO_STOP);  // stop servo
    }
    buf = Serial.read();
  }
  delay(50);

  
}