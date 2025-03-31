/*
 * DGP_Arduino_code
 *
 * written by jihoonkimtech
 * contributed by krin3434 (at gyro library)
 *
 * last modified at 2025-03-30
*/

//#include "library\DGP_Fields.h"
//#include "library\DGP_Servo.h"
//#include "library\DGP_Gyro.h"

#include <SoftwareSerial.h>
#include "library\DGP.h"

/* ==== MACROS ==== */
// pin number
#define BT_RX 2
#define BT_TX 3  // HC-06's PIN

#define SENSOR_L 5
#define SENSOR_R 6  // SENSOR's PIN

#define SERVO_L 10
#define SERVO_R 11  // SERVO's PIN

#define GYRO_SCL A4
#define GYRO_SDA A5  // MPU6050's PIN

#define Buzzer 8

#define LED 13      // HIGH = OFF

// etc.

/* ==== CONSTANT VALUE ==== */
const long interval = 200;  // LED interval time setting
/* ==== VARIABLES ==== */
uint8_t M_speeds[2][3] = { { 120, 150, 180 },   // [0][n] for ClockWise
                           { 60, 30, 0 } };     // [1][n] for Counter-ClockWise
uint8_t FM_speeds[2][3] = { { 100, 120, 140 },  // [0][n] for ClockWise
                            { 80, 60, 40 } };   // [1][n] for Counter-ClockWise
                                                // low, mid,  high
bool devMod = true;                             // toggle debug mode, if false = off

bool Sound = true;  // toggle Sound mode, if true buzzer on, if false led on, default mode is Sound
bool ledState = HIGH;
int toneStep = 0;   // trace every count of sound

unsigned long prevMillis = 0;  // last time when LED changed

/* ==== OBJECTS ==== */
DGP_Fields fields;  // fields info
DGP_Servo servo(SERVO_L, SENSOR_L,
                SERVO_R, SENSOR_R);       // for band winding
DGP_Gyro gyro(GYRO_SDA, GYRO_SCL, 1.50);  // for gyro sensing
SoftwareSerial btSerial(BT_TX, BT_RX);    // software serial for bluetooth

void setup() {
  if (devMod) Serial.begin(9600);  // for debug: serial start
  btSerial.begin(9600);            // bluetooth comm start
  servo.init();
  servo.setMaleRef(M_speeds);     // set male's motor power reference
  servo.setFemaleRef(FM_speeds);  // set female's motor power reference
  if (devMod) servo.printSerialRefs();
  gyro.init(true);                // initializing gyro

  pinMode(LED, OUTPUT);
  digitalWrite(LED, ledState);
}

void loop() {
  if (Serial.available()) {                // if the data came in via bluetooth
    String bufStr = "";                    // initialize buffer string
    bufStr = Serial.readStringUntil('.');  // read chars until came '.'(eof)

    char cmd = bufStr.charAt(0);

    switch (cmd) {
      case 's':
      case 'S':                                 // 'S' is Slience
        if (devMod) Serial.println("==== ACTIVE LED ====");
        Sound = false;                          // Sound off
        break;
      case 'b':
      case 'B':                                 // 'B' is Buzzer
        if (devMod) Serial.println("==== ACTIVE BUZZER ====");
        Sound = true;                           // Sound on
        break;
      case 'e':             // 'e' is unwind
        if (devMod) Serial.println("==== UNWIND ====");
        servo.unwinding();  // just unwind
        gyro.disableCali();
        break;

      default:
        fields.extractField(bufStr);  // extract fields from string
        if (devMod) fields.printSerialField();
        servo.setUser(fields.getOp(),
                      fields.getPL(),
                      fields.getPR());           // set user's mode
        if (devMod) servo.printSerialUsrInfo();  // for debug: display info

        servo.unwinding();                    // unwind before wind
        servo.winding();                      // wind
        gyro.calibration(true);               // calibration
        if (devMod) gyro.printSerialCali();   // for debug: display gyro
        break;
    }
  }

  if (gyro.getCali()) {  // if already calibration?
    while (!gyro.compareValue(true)) {
      unsigned long currentMillis = millis();

      if (toneStep == 0 && currentMillis - prevMillis >= 0) {         //millis calculate  
        if(Sound){    
          tone(Buzzer, 1000);  // first sound
          prevMillis = currentMillis;
          toneStep = 1;
        } else{
          digitalWrite(LED, LOW);
        }
      }

      else if (toneStep == 1 && currentMillis - prevMillis >= 150) {  //millis calculate   
        if(Sound){
          noTone(Buzzer);  // waiting
          prevMillis = currentMillis;
          toneStep = 2;
        } else{
          digitalWrite(LED, HIGH);
        }
      }

      else if (toneStep == 2 && currentMillis - prevMillis >= 50) {   //millis calculate
        if(Sound){
          tone(Buzzer, 1300);  // second sound
          prevMillis = currentMillis;
          toneStep = 3;
        } else{
          digitalWrite(LED, LOW);
        }
      }

      else if (toneStep == 3 && currentMillis - prevMillis >= 200) {   //millis calculate
        if(Sound){
          noTone(Buzzer);   // end
          toneStep = 1;
        } else{
          digitalWrite(LED, HIGH);
        }
      }

    }
    digitalWrite(LED, HIGH);
    noTone(Buzzer);
  }
  delay(10);
}
