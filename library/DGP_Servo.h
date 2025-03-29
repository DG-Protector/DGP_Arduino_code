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

#endif
