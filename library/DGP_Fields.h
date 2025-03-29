#ifndef _DGP_FIELDS_H   // header-guard
#define _DGP_FIELDS_H

#include "DGP.h"

/*
 * library of field dividing
 *
 * written by jihoonkimtech
 *
 * last modified at 2025-03-29
*/

/* about fields
 * |op  |paramL  |separtor |paramR |eof  |
 * op is operator (it's char)
    - 'u' or 'U' : user setting.
    - 'l' or 'L' : low power setting for female(lowercase) or male(uppercase). no need param.
    - 'm' or 'M : middle power setting for female(lowercase) or male(uppercase). no need param.
    - 'h' or 'H' : high power setting for female(lowercase) or male(uppercase). no need param.
 * paramL is parameter of left motor (it's short)
    - only values between 0 and 100
 * separtor is for distinguish between parameters (it's always (char)',')
 * paramR is parameter of right motor (it's short)
    - only values between 0 and 100
 * eof is for point end of field (it's always (char)'.')
*/
class DGP_Fields {
    public:
        char op;                    // operator
        uint8_t paramLeft = 0;      // parameter of left servo
        uint8_t paramRight = 0;     // parameter of left servo
        void extractField(String str);
        void printSerialField();
}

void DPG_Fields::extractField(String str){  // extract fields from string
    struct Fields f;

  // read op
  op = str.charAt(0);          // copy op field
  str.remove(0,1);               // remove op in origin fields

    bool isNeedParams = false;
    switch(op){   // check need param
        case 'l':   // case of low power for female setting
        case 'L':   // case of low power for male setting
        case 'm':   // case of middle power for female setting
        case 'M':   // case of middle power for male setting
        case 'h':   // case of high power for female setting
        case 'H':   // case of high power for male setting
            break;  // no need param
        case 'u':
        case 'U':                   // case of user setting
            isNeedParams = true;    // need param
            break;
        default:
            break;
    }

    if(isNeedParams){
        uint8_t ref = 0;
        ref = str.indexOf(',');                         // check index for separator
        paramLeft = str.substring(0, ref).toInt();    // extract paramL
        str.remove(0, ref+1);                           // remove paramL & separator

        ref = str.indexOf('.');                         // check index for end of field
        paramRight = str.substring(0, ref).toInt();   // extract paramR
        str.remove(0, ref+1);                           // remove paramR & end of field
    }
    return;
}

void DPG_Fields::printSerialField(){      // display fields
    Serial.println("=====start=====");
    Serial.print("op : ");
    Serial.println(op);
    Serial.print("pL : ");
    Serial.println(paramLeft);
    Serial.print("pR : ");
    Serial.println(paramRight);
    Serial.println("======end======");
}



#endif
