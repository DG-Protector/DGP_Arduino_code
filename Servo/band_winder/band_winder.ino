/*
 * band winder
 *
 * written by jihoonkimtech
 *
 * last modified at 2025-03-28
*/

#include <Servo.h>            // library of servo

Servo L_SERVO;                // object of Servo class
Servo R_SERVO;

#define L_SV  10              // servo motors
#define R_SV  11
#define L_SS  5               // sensors
#define R_SS  6

const short SERVO_FRWD = 100; // rotate clockwise servo
const short SERVO_BKWD = 60;  // rotate anti-clockwise servo
const short SERVO_STOP = 90;  // stop rotate servo

void setup() {
  Serial.begin(115200);       // for debug

  pinMode(L_SS, INPUT);       // pinmode set-up
  pinMode(R_SS, INPUT);

  L_SERVO.attach(L_SV);       // attach servo
  R_SERVO.attach(R_SV);
  L_SERVO.write(SERVO_STOP);  // stop servo
  R_SERVO.write(SERVO_STOP);

  servo_init();               // initialize servos
}

void loop() {
  if(Serial.available()){
    int tmp = Serial.parseInt();
    Serial.println("Start=====");
    band_winding(tmp);
    Serial.println("End=====");
    tmp = Serial.parseInt();
  }
  
}

bool _checkSensor(int pin){
  bool prev = digitalRead(pin);  // check value
  delay(20);
  bool next = digitalRead(pin);
  delay(20);
  bool now = digitalRead(pin);
  
  if(prev == next == now)         // read value check
    return now;
  else
    return false;
}

void servo_init() {               // initializing servo motor
  if(_checkSensor(L_SS)){;}       // already reached the point of reference? -> end.
  else {
    L_SERVO.write(SERVO_BKWD);    // servo rotate
    while(!_checkSensor(L_SS));   // break if sensor find point of reference
    L_SERVO.write(SERVO_STOP);    // stop servo
  }

  if(_checkSensor(R_SS)){;}
  else {
    R_SERVO.write(SERVO_FRWD); 
    while(!_checkSensor(R_SS));
    R_SERVO.write(SERVO_STOP);
  }
  
  return;
}

void band_winding(short num) {        // choice wind or unwind
  servo_init();

  short set_direction;
  if(num > 0) band_wind(num);
  else band_unwind(num);

  return;
}

void band_wind(short num){            // winding band
  for(short x = 0; x < num; x++){
    for(short i = 0; i < 4; i++) {
      L_SERVO.write(SERVO_BKWD);
      while(_checkSensor(L_SS));      // rotation until out of line
      L_SERVO.write(SERVO_STOP);

      L_SERVO.write(SERVO_BKWD);
      while(!_checkSensor(L_SS));     // rotation until entering the line 
      L_SERVO.write(SERVO_STOP);

      R_SERVO.write(SERVO_FRWD);
      while(_checkSensor(R_SS));      // rotation until out of line
      R_SERVO.write(SERVO_STOP);

      R_SERVO.write(SERVO_FRWD);
      while(!_checkSensor(R_SS));     // rotation until entering the line 
      R_SERVO.write(SERVO_STOP);
    }
  }
  
  return;
}

void band_unwind(short num){            // unwinding band
  num *= -1;

  for(short x = 0; x < num; x++){
    for(short i = 0; i < 4; i++) {
      L_SERVO.write(SERVO_FRWD);
      while(_checkSensor(L_SS));        // rotation until out of line
      L_SERVO.write(SERVO_STOP);

      L_SERVO.write(SERVO_FRWD);
      while(!_checkSensor(L_SS));       // rotation until entering the line 
      L_SERVO.write(SERVO_STOP);

      R_SERVO.write(SERVO_BKWD);
      while(_checkSensor(R_SS));        // rotation until out of line
      R_SERVO.write(SERVO_STOP);
      
      R_SERVO.write(SERVO_BKWD);
      while(!_checkSensor(R_SS));       // rotation until entering the line 
      R_SERVO.write(SERVO_STOP);
    }
  }
  
  return;
}
