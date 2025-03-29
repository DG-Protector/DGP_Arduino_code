/*
 * HC-06 Setup
 *
 * written by jihoonkimtech
 *
 * last modified at 2025-03-28
*/

/*
 * using AT command. can setting HC-06
 * open Serial monitor and set Both NL&CR (line ending setting)
 * First, check version
   - AT+VERSION
 * if version is 1.8?
   - AT+NAME[name]: change name
   - AT+PIN[passwd]: change password
   - AT+ROLE=[M or S]: change mode to (M)aster or (S)lave
 * if version is 3.0?
   - AT+NAME=[name]: change name
   - AT+PSWD=[passwd]: change password
   - AT+ROLE=[1 or 0]: change mode to Master(1) or Slave(0)

 * setting template
 * NAME = DG_PROTECTOR
 * PASSWORD = 2025
 * MODE = SLAVE
*/

#include <SoftwareSerial.h>

#define BT_TX     3
#define BT_RX     2     // hc-06's pin

#define BRATE_DEF 9600  // HC-06's default baudrate

SoftwareSerial btSerial(BT_TX, BT_RX);  //bluetooth serial object

void setup() {
  btSerial.begin(BRATE_DEF);  // begin software serial
  Serial.begin(BRATE_DEF);    // begin hardware serial
}

void loop() {
  if(btSerial.available()){          // btSerial to Serial
    Serial.write(btSerial.read());
  }
  if(Serial.available()){            // Serial to btSerial
    btSerial.write(Serial.read());
  }
}
