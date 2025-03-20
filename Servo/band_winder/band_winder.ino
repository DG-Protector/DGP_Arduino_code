/*
 * band winder
 *
 * written by jihoonkimtech
 *
 * last modified at 2025-03-20
*/

#include <Servo.h>    // library of servo
Servo L_SERVO;          // object of Servo class
Servo R_SERVO;

#define L_SV_PIN 10   // servo motors
#define R_SV_PIN 10
#define L_SS_PIN 7    // sensors
#define R_SS_PIN 6

#define SERVO_FRWD 110  // rotate clockwise servo
#define SERVO_BKWD 70   // rotate anti-clockwise servo
#define SERVO_STOP 90   // stop rotate servo

void setup() {
  Serial.begin(115200);       // for debug

  pinMode(L_SS_PIN, INPUT);   // pinmode set-up
  pinMode(R_SS_PIN, INPUT);

  L_SERVO.attach(L_SV_PIN);   // attach servo
  R_SERVO.attach(R_SV_PIN);
  L_SERVO.write(SERVO_STOP);  // stop servo
  R_SERVO.write(SERVO_STOP);
  servo_init();
}

void loop() {
  if(Serial.available()){
    int tmp = Serial.parseInt();
    band_winding(tmp);
  }
  
}

bool _checkSensor(int pin){
  bool prev = digitalRead(pin);  // check value
  delay(50);
  bool next = digitalRead(pin);
  delay(50);
  bool now = digitalRead(pin);
  
  if(prev == next == now)       // read value check
    return now;
  else
    return false;
}

void servo_init() {       // initializing servo motor (find line)
  if(_checkSensor(L_SS_PIN));
  else {
    L_SERVO.write(SERVO_BKWD);         // servo rotate
    while(!_checkSensor(L_SS_PIN));    // break if it's initialized
    L_SERVO.write(SERVO_STOP);
  }

  if(_checkSensor(R_SS_PIN));
  else {
    R_SERVO.write(SERVO_FRWD); 
    while(!_checkSensor(R_SS_PIN));
    R_SERVO.write(SERVO_STOP);
  }
  
  return;
}

void band_winding(int num) {   // choice wind or unwind
  servo_init();

  int set_direction;
  if(num > 0) band_wind(num);
  else band_unwind(num);

  return;
}

void band_wind(int num){            // winding band
  for(int x = 0; x < num; x++){
    for(int i = 0; i < 4; i++) {
      L_SERVO.write(SERVO_FRWD);
      while(_checkSensor(L_SS_PIN)); // rotation until out of line
      L_SERVO.write(SERVO_STOP);

      R_SERVO.write(SERVO_FRWD);
      while(_checkSensor(R_SS_PIN)); // rotation until out of line
      R_SERVO.write(SERVO_STOP);

      L_SERVO.write(SERVO_FRWD);
      while(!_checkSensor(L_SS_PIN)); // rotation until entering the line 
      L_SERVO.write(SERVO_STOP);

      R_SERVO.write(SERVO_FRWD);
      while(!_checkSensor(R_SS_PIN)); // rotation until entering the line 
      R_SERVO.write(SERVO_STOP);
    }
  }
  
  return;
}

void band_unwind(int num){            // unwinding band
  for(int x = 0; x < num; x++){
    for(int i = 0; i < 4; i++) {
      L_SERVO.write(SERVO_BKWD);
      while(_checkSensor(L_SS_PIN)); // rotation until out of line
      L_SERVO.write(SERVO_STOP);

      R_SERVO.write(SERVO_BKWD);
      while(_checkSensor(R_SS_PIN)); // rotation until out of line
      R_SERVO.write(SERVO_STOP);

      L_SERVO.write(SERVO_BKWD);
      while(!_checkSensor(L_SS_PIN)); // rotation until entering the line 
      L_SERVO.write(SERVO_STOP);

      R_SERVO.write(SERVO_BKWD);
      while(!_checkSensor(R_SS_PIN)); // rotation until entering the line 
      R_SERVO.write(SERVO_STOP);
    }
  }
  
  return;
}
