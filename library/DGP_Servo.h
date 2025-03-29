#ifndef _DGP_SERVO_H   // header-guard
#define _DGP_SERVO_H

#include "DGP.h"
#include <Servo.h>
/*
 * library of band winding using servo
 *
 * written by jihoonkimtech
 *
 * last modified at 2025-03-29
*/

#define R   1   // right's index
#define L   0   // low's or left index
#define M   1   // mid's index
#define H   2   // high's index

#define CW  0   // CW's index
#define CCW 1   // CCW's index

#define MALE    0   // male's indicator
#define FEMALE  1   // female's indicator

#define TIME_OUT    5000    // timeout ref

#define SERVO_STP   90      // servo stop

class DGP_Servo {
    private:
        Servo servo[2];
        uint8_t servo_pin[2];
        uint8_t sensor[2];
        uint8_t howManyWind[2] = {0, 0};                    // count per 1/4 cycle = 1 step {L, R}

        // motor power reference by gender
        uint8_t malesPower[2][3] = { {0,   0,    0},        // [0][n] for ClockWise
                                     {0,   0,    0} };      // [1][n] for Counter-ClockWise                    
        uint8_t femalesPower[2][3] = { {0,   0,    0},      // [0][n] for ClockWise
                                       {0,   0,    0} };    // [1][n] for Counter-ClockWise
                                    // low, mid,  high

        boolean isCustom = false;                           // if high? op is 'u'(custom mode)
        uint8_t usrPowerArr[2][2] = { { 0, 0 },             // user's custom setting if custom mode? -> {L CW, LCCW}, {R CW, R CCW}
                                      { 0, 0 } };           // else? -> {CW, CCW}

        boolean getSensorValue(uint8_t pin);                // read senser's sensing value
        void findPointOfRef();                              // find point of reference

        boolean rotateOneStep(boolean d, boolean c);        // rotate just one step         //if 0, fail(timeout)
        boolean rotateUntilFindPoint(boolean d, boolean c); // rotate until find of point   //if 0, fail(timeout)
        boolean rotateUntilOutPoint(boolean d, boolean c);  // rotate until out of point    //if 0, fail(timeout)
    public:
        DGP_Servo(){;}
        DGP_Servo(  uint8_t l_servo_pin, uint8_t l_sensor_pin,      // initializing
                    uint8_t r_servo_pin, uint8_t r_sensor_pin   );
        ~DGP_Servo(){;}
        void init();
        void setMaleRef(uint8_t arr[2][3]);             // set male's motor power reference
        void setFemaleRef(uint8_t arr[2][3]);           // set female's motor power reference
        void setUser(char op, uint8_t pL,uint8_t pR);   // set user's power
        void winding();                                 // winding
        void unwinding();                               // unwinding
        void printSerialRefs();                         // for debug
        void printSerialUsrInfo();                      // for debug
};


DGP_Servo::DGP_Servo(   uint8_t l_servo_pin, uint8_t l_sensor_pin,      // initializing
                        uint8_t r_servo_pin, uint8_t r_sensor_pin   ){
    servo_pin[L] = l_servo_pin;
    servo_pin[R] = r_servo_pin;
    sensor[L] = l_sensor_pin;
    sensor[R] = r_sensor_pin;
    
}

void DGP_Servo::init(){
    servo[L].attach( servo_pin[L]);
    servo[L].write(90);

    servo[R].attach(servo_pin[R]);
    servo[R].write(90);

    pinMode(sensor[L], INPUT);
    pinMode(sensor[R], INPUT);
}

void DGP_Servo::setMaleRef(uint8_t arr[2][3]){      // set male's motor power reference
    malesPower[CW][L] = arr[CW][L];
    malesPower[CCW][L] = arr[CCW][L];

    malesPower[CW][M] = arr[CW][M];
    malesPower[CCW][M] = arr[CCW][M];

    malesPower[CW][H] = arr[CW][H];
    malesPower[CCW][H] = arr[CCW][H];
    
    return;
}

void DGP_Servo::setFemaleRef(uint8_t arr[2][3]){    // set female's motor power reference
    femalesPower[CW][L] = arr[CW][L];
    femalesPower[CCW][L] = arr[CCW][L];

    femalesPower[CW][M] = arr[CW][M];
    femalesPower[CCW][M] = arr[CCW][M];

    femalesPower[CW][H] = arr[CW][H];
    femalesPower[CCW][H] = arr[CCW][H];
    
    return;
}

void DGP_Servo::setUser(char op, uint8_t pL, uint8_t pR){   
    isCustom = false;                                   // initializing mode toggle
    switch(op){
        case 'l':   // case of low power for female setting
            usrPowerArr[L][CW] = usrPowerArr[R][CW] = femalesPower[CW][L];
            usrPowerArr[L][CCW] = usrPowerArr[R][CCW] = femalesPower[CCW][L];
            break;
        case 'L':   // case of low power for male setting
            usrPowerArr[L][CW] = usrPowerArr[R][CW] = femalesPower[CW][L];
            usrPowerArr[L][CCW] = usrPowerArr[R][CCW] = femalesPower[CCW][L];
            break;
        case 'm':   // case of middle power for female setting
            usrPowerArr[L][CW] = usrPowerArr[R][CW] = femalesPower[CW][M];
            usrPowerArr[L][CCW] = usrPowerArr[R][CCW] = femalesPower[CCW][M];
            break;
        case 'M':   // case of middle power for male setting
            usrPowerArr[L][CW] = usrPowerArr[R][CW] = malesPower[CW][M];
            usrPowerArr[L][CCW] = usrPowerArr[R][CCW] = malesPower[CCW][M];
            break;
        case 'h':   // case of high power for female setting
            usrPowerArr[L][CW] = usrPowerArr[R][CW] = femalesPower[CW][H];
            usrPowerArr[L][CCW] = usrPowerArr[R][CCW] = femalesPower[CCW][H];
            break;
        case 'H':   // case of high power for male setting
            usrPowerArr[L][CW] = usrPowerArr[R][CW] = malesPower[CW][H];
            usrPowerArr[L][CCW] = usrPowerArr[R][CCW] = malesPower[CCW][H];
            break;
        case 'u':
        case 'U':   // case of user setting
            // < 90 = CW, > 90 = CCW.. fast as far from 90.
            isCustom = true;
            uint8_t biasL = map(pL, 0, 100, 0, 90);     // calculate bias
            uint8_t biasR = map(pR, 0, 100, 0, 90); 
            usrPowerArr[L][CW] = SERVO_STP + biasL;     // 90 + n (if n > 0)? it will rotate CW
            usrPowerArr[L][CCW] = SERVO_STP - biasL;    // 90 - n (if n > 0)? it will rotate CCW
            usrPowerArr[R][CW] = SERVO_STP + biasR;
            usrPowerArr[R][CCW] = SERVO_STP - biasR;
            break;
    }

    return;
}

boolean DGP_Servo::getSensorValue(uint8_t pin){     // read senser's sensing value
    boolean t0 = digitalRead(pin);
    delay(10);
    boolean t1 = digitalRead(pin);

    if(t0 != t1){                                   // debouncing
        delay(10);
        t0 = digitalRead(pin);
    }

    return t0;
    
}

boolean DGP_Servo::rotateUntilFindPoint(boolean d, boolean c){  // rotate until find of point
    if(getSensorValue(sensor[d])){;}        // already reached the point of reference? -> end.
    else {
        servo[d].write(usrPowerArr[d][c]);  // servo rotate
        uint16_t count = 0;
        while(!getSensorValue(sensor[d])){  // break if sensor find point of reference
            delay(1); count++;              // count
            if(count > TIME_OUT){           // time out!
                servo[d].write(SERVO_STP);  // stop servo
                return false;               // fail
            }
        }   
    }
    servo[d].write(SERVO_STP);              // stop servo
    return true;                            // no time out? success
}

boolean DGP_Servo::rotateUntilOutPoint(boolean d, boolean c){   // rotate until out of point
    if(!getSensorValue(sensor[d])){;}        // already reached the point of reference? -> end.
    else {
        servo[d].write(usrPowerArr[d][c]);  // servo rotate
        uint16_t count = 0;
        while(getSensorValue(sensor[d])){   // break if sensor find point of reference
            delay(1); count++;              // count
            if(count > TIME_OUT){           // time out!
                servo[d].write(SERVO_STP);  // stop servo
                return false;               // fail
            }
        } 
    }
    //servo[d].write(SERVO_STP);              // stop servo
    return true;                            // no time out? success
}

void DGP_Servo::findPointOfRef(){           // find point of reference
    rotateUntilFindPoint(L, CW);
    rotateUntilFindPoint(R, CCW);
}

boolean DGP_Servo::rotateOneStep(boolean d, boolean c){    // find point of reference
    if(!rotateUntilFindPoint(d, c))
        return false;
    if(!rotateUntilOutPoint(d, c))
        return false;
    return true;                                            // no time out? -> true
}

void DGP_Servo::winding(){                  // winding
    findPointOfRef();

    boolean L_TO = false, R_TO = false;     // check time out 
    while ((!L_TO || !R_TO)){               // repeat until if a timeout occurs L & R
        if(!L_TO){                          // rotate if not occurred time out
            if(rotateOneStep(L, CCW))       // wind
                howManyWind[L]++;           // count 
            else                            // if occurred time out?
                L_TO = true;                // set
        }
        if(!R_TO){
            if(rotateOneStep(R, CW))
                howManyWind[R]++;
            else
                R_TO = true;
        }
    }
}

void DGP_Servo::unwinding(){                        // unwinding
    findPointOfRef();

    while (!(howManyWind[L] == 0 || howManyWind[L] == 0)){  // repeat until if unwind done
        if(rotateOneStep(L, CW))                            // unwind
            howManyWind[L]--;                               // count down

        if(rotateOneStep(R, CCW))
            howManyWind[R]--;
    }

    servo[L].write(SERVO_STP);
    servo[R].write(SERVO_STP);
}

void DGP_Servo::printSerialRefs(){                  // for debug
    Serial.println("==== DISPLAY REFS ====");
    Serial.println("[malesPower CW]");
    Serial.print(malesPower[CW][L]); Serial.print(','); Serial.print(malesPower[CW][M]); Serial.print(','); Serial.println(malesPower[CW][H]);
    Serial.println("[malesPower CCW]");
    Serial.print(malesPower[CCW][L]); Serial.print(','); Serial.print(malesPower[CCW][M]); Serial.print(','); Serial.println(malesPower[CCW][H]);
    Serial.println("[femalesPower CW]");
    Serial.print(femalesPower[CW][L]); Serial.print(','); Serial.print(femalesPower[CW][M]); Serial.print(','); Serial.println(femalesPower[CW][H]);
    Serial.println("[femalesPower CCW]");
    Serial.print(femalesPower[CCW][L]); Serial.print(','); Serial.print(femalesPower[CCW][M]); Serial.print(','); Serial.println(femalesPower[CCW][H]);
    Serial.println("==== END ====");
        
    return;
}

void DGP_Servo::printSerialUsrInfo(){               // for debug
    Serial.println("==== DISPLAY USER INFO ====");
    if(isCustom){
        Serial.println("[usrPower L CW, L CCW, R CW, R CCW]");
        Serial.print(usrPowerArr[L][CW]); Serial.print(','); Serial.println(usrPowerArr[L][CCW]);
        Serial.print(usrPowerArr[R][CW]); Serial.print(','); Serial.println(usrPowerArr[R][CCW]);
    }
    else {
        Serial.println("[usrPower CW, CCW]");
        Serial.print(usrPowerArr[0][CW]); Serial.print(','); Serial.println(usrPowerArr[0][CCW]);
    }
    Serial.println("==== END ====");
        
    return;
}



#endif
