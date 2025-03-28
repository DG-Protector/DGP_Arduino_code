/*
 * bluetooth serial tester for HC-06
 *
 * written by jihoonkimtech
 *
 * last modified at 2025-03-28
*/

#include <SoftwareSerial.h>

struct Fields{
  char op;
  short paramLeft = 0;
  short paramRight = 0;
};

#define BT_TX     3
#define BT_RX     2     // hc-06's pin

#define BRATE_DEF 9600  // HC-06's default baudrate

SoftwareSerial btSerial(BT_TX, BT_RX);  //bluetooth serial object

struct Fields F;

void setup() {
  btSerial.begin(BRATE_DEF);  // begin software serial
  Serial.begin(9600);         // begin hardware serial

}

void loop() {
  if(btSerial.available()){
    String cmd = btSerial.readString();
    F = extractField(cmd);
    printSerialField(F);
  }
}

// funcion for extrat field at string
struct Fields extractField(String str){
  struct Fields f;

  // read op
  f.op = str.charAt(0);          // copy op field
  str.remove(0,1);               // remove op in origin fields

  bool isNeedParams = false;
  switch(f.op){ // check need param
    case 'l':   // case of low power for female setting
    case 'L':   // case of low power for male setting
    case 'm':   // case of middle power for female setting
    case 'M':   // case of middle power for male setting
    case 'h':   // case of high power for female setting
    case 'H':   // case of high power for male setting
                // no need param
      break;
    case 'u':
    case 'U':               // case of user setting
      isNeedParams = true;  // need param
      break;
    default:
      break;
  }

  if(isNeedParams){
    short ref = 0;
    ref = str.indexOf(',');                          // check index for separator
    f.paramLeft = str.substring(0, ref).toInt(); // extract paramL
    str.remove(0, ref+1);                            // remove paramL & separator

    ref = str.indexOf('.');                          // check index for end of field
    f.paramRight = str.substring(0, ref).toInt();// extract paramR
    str.remove(0, ref+1);                            // remove paramR & end of field
    }
    return f;
}

// print fields for debug
void printSerialField(struct Fields f){
  Serial.println("=====start=====");
  Serial.print("op : ");
  Serial.println(f.op);
  Serial.print("pL : ");
  Serial.println(f.paramLeft);
  Serial.print("pR : ");
  Serial.println(f.paramRight);
  Serial.println("======end======");
}
