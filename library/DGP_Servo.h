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

#define SERVO_STP   90  // servo stop

class DGP_Servo {
    private:
        Servo leftServo;
        Servo rightServo;
        uint8_t leftSensor;
        uint8_t rightSensor;
        uint8_t howManyWind = 0;    // count per 1/4 cycle

        // motor power reference by gender
        uint8_t malesPower[2][3] = { {0,   0,    0},        // [0][n] for ClockWise
                                     {0,   0,    0} };      // [1][n] for Counter-ClockWise                    
        uint8_t femalesPower[2][3] = { {0,   0,    0},      // [0][n] for ClockWise
                                       {0,   0,    0} };    // [1][n] for Counter-ClockWise
                                    // low, mid,  high

        boolean isCustom = false;                           // if high? op is 'u'(custom mode)
        uint8_t usrPowerArr[2][2] = { { 0, 0 },             // user's custom setting if custom mode? -> {L CW, LCCW}, {R CW, R CCW}
                                      { 0, 0 } };           // else? -> {CW, CCW}

        boolean getSensorValue(uint8_t pin);    // read senser's sensing value
        void findPointOfRef();                  // find point of reference
        void winding();                         // winding Counter ClockWise
        void unwinding();                       // winding Counter Clock
        void rotateUntilFindPoint();
        void rotateUntilOutPoint();
    public:
        DGP_Servo(){;}
        DGP_Servo(  uint8_t l_servo_pin, uint8_t l_sensor_pin,      // initializing
                    uint8_t r_servo_pin, uint8_t r_sensor_pin   );
        ~DGP_Servo(){;}
        void setMaleRef(uint8_t arr[2][3]);             // set male's motor power reference
        void setFemaleRef(uint8_t arr[2][3]);           // set female's motor power reference
        void setUser(char op, uint8_t pL,uint8_t pR);   // set user's power
        void bandWinding();                             // band winding
        void printSerialRefs();                         // for debug
        void printSerialUsrInfo();                      // for debug
};


DGP_Servo::DGP_Servo(   uint8_t l_servo_pin=0, uint8_t l_sensor_pin=0,      // initializing
                        uint8_t r_servo_pin=0, uint8_t r_sensor_pin=0   ){
    leftServo.attach(l_servo_pin);
    rightServo.attach(r_servo_pin);

    pinMode(l_sensor_pin, INPUT);
    pinMode(r_sensor_pin, INPUT);
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
            usrPowerArr[0][CW] = femalesPower[CW][L];
            usrPowerArr[0][CCW] = femalesPower[CCW][L];
            break;
        case 'L':   // case of low power for male setting
            usrPowerArr[0][CW] = femalesPower[CW][L];
            usrPowerArr[0][CCW] = femalesPower[CCW][L];
            break;
        case 'm':   // case of middle power for female setting
            usrPowerArr[0][CW] = femalesPower[CW][M];
            usrPowerArr[0][CCW] = femalesPower[CCW][M];
            break;
        case 'M':   // case of middle power for male setting
            usrPowerArr[0][CW] = malesPower[CW][M];
            usrPowerArr[0][CCW] = malesPower[CCW][M];
            break;
        case 'h':   // case of high power for female setting
            usrPowerArr[0][CW] = femalesPower[CW][H];
            usrPowerArr[0][CCW] = femalesPower[CCW][H];
            break;
        case 'H':   // case of high power for male setting
            usrPowerArr[0][CW] = malesPower[CW][H];
            usrPowerArr[0][CCW] = malesPower[CCW][H];
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
#endif
