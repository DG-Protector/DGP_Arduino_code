/*
 * DGP_Arduino_code
 *
 * written by jihoonkimtech
 *
 * last modified at 2025-03-28
*/

//#include "library/DGP_Fields.h"
//#include "library/DGP_Servo.h"
//#include "library/DGP_Gyro.h"

#include <SoftwareSerial.h>
#include "library\DGP.h"

/* ==== OBJECTS ==== */
DGP_Fields fields;                      // fields info
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
