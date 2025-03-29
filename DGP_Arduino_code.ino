/*
 * DGP_Arduino_code
 *
 * written by jihoonkimtech
 *
 * last modified at 2025-03-28
*/

//#include "library\DGP_Fields.h"
//#include "library\DGP_Servo.h"
//#include "library\DGP_Gyro.h"

#include <SoftwareSerial.h>
#include "library\DGP.h"

/* ==== MACROS ==== */
// pin number
#define BT_RX     2
#define BT_TX     3   // HC-06's PIN

#define SENSOR_L  5
#define SENSOR_R  6   // SENSOR's PIN

#define SERVO_L   10
#define SERVO_R   11  // SERVO's PIN

#define GYRO_INT  4
#define GYRO_SCL  A4
#define GYRO_SDA  A5  // MPU6050's PIN

// etc.

/* ==== CONSTANT VALUE ==== */
                                // low, mid,  high
const bool devMod = true;       // toggle debug mode, if false = off

/* ==== OBJECTS ==== */
DGP_Fields  fields;                     // fields info
SoftwareSerial btSerial (BT_TX, BT_RX); // software serial for bluetooth

void setup() {
  if(devMod) Serial.begin(9600);  // for debug: serial start
  btSerial.begin(9600);           // bluetooth comm start
}

void loop() {
  if(btSerial.available()){                 // if the data came in via bluetooth
    String bufStr = "";                     // initialize buffer string                      
    bufStr = btSerial.readStringUntil('.'); // read chars until came '.'(eof)
    fields.extractField(bufStr);            // extract fields from string
    if(devMod) fields.printSerialField();   // for debug: display fields
  }
  delay(100);
}
