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
#include <EEPROM.h>
#include "library\DGP.h"

/* ==== MACROS ==== */
// pin number
#define BT_RX 2
#define BT_TX 3     // HC-06's PIN

#define SENSOR_L 5
#define SENSOR_R 6  // SENSOR's PIN

#define SERVO_L 10
#define SERVO_R 11  // SERVO's PIN

#define GYRO_SCL A4
#define GYRO_SDA A5 // MPU6050's PIN

#define Buzzer 8

#define LED 13      // HIGH = OFF

// etc.
#define IDX_MODE 0  // index of mode state

/* ==== CONSTANT VALUE ==== */
const long interval = 200;  // LED interval time setting
/* ==== VARIABLES ==== */
uint8_t M_speeds[2][3] = { { 120, 150, 180 },   // [0][n] for ClockWise
                           { 60, 30, 0 } };     // [1][n] for Counter-ClockWise
uint8_t FM_speeds[2][3] = { { 100, 120, 140 },  // [0][n] for ClockWise
                            { 80, 60, 40 } };   // [1][n] for Counter-ClockWise
                                                // low, mid,  high
bool devMod = true;                             // toggle debug mode, if false = off

bool soundMode = true;  // toggle Sound mode, if true buzzer on, if false led on, default mode is Sound
bool ledState = HIGH;
int toneStep = 0;       // trace every count of sound

unsigned long prevMillis = 0;  // last time when LED changed

/* ==== OBJECTS ==== */
DGP_Fields fields;  // fields info
DGP_Servo servo(SERVO_L, SENSOR_L,
                SERVO_R, SENSOR_R);       // for band winding
DGP_Gyro gyro(GYRO_SDA, GYRO_SCL, 1.50);  // for gyro sensing
SoftwareSerial btSerial(BT_TX, BT_RX);    // software serial for bluetooth

void setup() {
  if (devMod) Serial.begin(9600);     // for debug: serial start
  btSerial.begin(9600);               // bluetooth comm start

  servo.init();
  servo.setMaleRef(M_speeds);         // set male's motor power reference
  servo.setFemaleRef(FM_speeds);      // set female's motor power reference
  if (devMod) servo.printSerialRefs();

  gyro.init(true);                    // initializing gyro

  pinMode(LED, OUTPUT);
  digitalWrite(LED, ledState);        // led pin setting

  soundMode = EEPROM.read(IDX_MODE);  // initial read mode state
}

void loop() {
  if (btSerial.available()) {                // if the data came in via bluetooth
    String bufStr = "";                    // initialize buffer string
    bufStr = btSerial.readStringUntil('.');  // read chars until came '.'(eof)

    char cmd = bufStr.charAt(0);

    switch (cmd) {
      case 's':
      case 'S':                                   // 'S' is mode indicator of silence
        if (devMod) Serial.println("==== ACTIVE LED ====");
        soundMode = false;                        // toggle
        EEPROM.write(IDX_MODE, soundMode);        // save state to EEPROM
        break;
      case 'b':
      case 'B':                                   // 'B' is mode indicator of buzzer
        if (devMod) Serial.println("==== ACTIVE BUZZER ====");
        soundMode = true;                         // toggle
        EEPROM.write(IDX_MODE, soundMode);        // save state to EEPROM
        break;
      case 'e':
      case 'E':                                   // 'e' is unwind band
        if (devMod) Serial.println("==== UNWIND ====");
        servo.unwinding();                        // just unwind
        gyro.disableCali();                       // disable calibration data
        break;

      default:
        fields.extractField(bufStr);              // extract fields from string
        if (devMod) fields.printSerialField();
        servo.setUser(fields.getOp(),             // set user via fields
                      fields.getPL(),
                      fields.getPR());            // set user's mode
        if (devMod) servo.printSerialUsrInfo();   // for debug: display info

        servo.unwinding();                        // unwind before wind
        servo.winding();                          // wind band
        gyro.calibration(true);                   // calibration, param is debug mode(true == on)
        if (devMod) gyro.printSerialCali();       // for debug: display gyro
        break;
    }
  }

  if (gyro.getCali()) {                       // if already calibration?
  while (!gyro.checkPosture(true)) {          // check users posture, param is debug mode(true == on)
      unsigned long currentMillis = millis();

      if (toneStep == 0 && currentMillis - prevMillis >= 0) {   //millis calculate
        if(soundMode){                    // ON
          tone(Buzzer, 1000);
          prevMillis = currentMillis;
          toneStep = 1;
        } else{
          digitalWrite(LED, LOW);
        }
      }

      else if (toneStep == 1 && currentMillis - prevMillis >= 100) {  //millis calculate   
        if(soundMode){                    // OFF
          noTone(Buzzer);
          prevMillis = currentMillis;
          toneStep = 2;
        } else{
          digitalWrite(LED, HIGH);
        }
      }

      else if (toneStep == 2 && currentMillis - prevMillis >= 150) {   //millis calculate
        if(soundMode){                    // OFF
          tone(Buzzer, 1300);
          prevMillis = currentMillis;
          toneStep = 3;
        } else{
          digitalWrite(LED, LOW);
        }
      }

      else if (toneStep == 3 && currentMillis - prevMillis >= 200) {   //millis calculate
        if(soundMode){                    // ON
          noTone(Buzzer);
          toneStep = 1;
        } else{
          digitalWrite(LED, HIGH);
        }
      }

    }
    digitalWrite(LED, HIGH);          // OFF indicators
    noTone(Buzzer);
  }
  delay(10);
}
