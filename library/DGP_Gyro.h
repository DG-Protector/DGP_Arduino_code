#ifndef _DGP_GYRO_H   // header-guard
#define _DGP_GYRO_H

/*
 * library of gyro sensing
 *
 * written by jihoonkimtech
 * contributed by krin3434
 *
 * last modified at 2025-03-30
*/

#include "DGP.h"
#include <Wire.h>
#include <MPU6050.h>

#define CALC_SAMPLES 10
#define CALI_SAMPLES 20

class DGP_Gyro{
    private:
        MPU6050 gyro;

        float thRef;                // threshold reference
        float thhold_x;
        float thhold_y;

        uint8_t sda_pin;
        uint8_t scl_pin;
        
        int16_t calc_x;
        int16_t calc_y;             // calculated value

        int16_t cali_x;
        int16_t cali_y;             // calibration value

        boolean isCompCali = false; //check for cali

        void refresh();             // re-calculate x, y
    public: 
        DGP_Gyro(){;}
        DGP_Gyro(uint8_t d, uint8_t c, float t = 0.95);
        ~DGP_Gyro(){;}

        void init();
        int16_t getX(){ return calc_x; }
        int16_t getY(){ return calc_y; }
        boolean getCali(){ return isCompCali; }
        void calibration();                     // calibration and set cali_x, cali_y
        void disableCali();                     // disable calibration
        boolean compareValue();                 // compare cali and calc
};

DGP_Gyro::DGP_Gyro(uint8_t d, uint8_t c, float t = 0.95){
    sda_pin = d;
    scl_pin = c;
    thRef = t;
}

void DGP_Gyro::init(){
    Wire.begin();       // wire communication start
    gyro.initialize();  // gyro module init

    delay(1000);        // connection wait
    
    return;
}

void DGP_Gyro::refresh(){                               // refresh value
    int16_t stackX = 0;
    int16_t stackY = 0;

    for(uint8_t i = 0; i < CALC_SAMPLES; i++){
        int16_t ax, ay, az;
        int16_t gx, gy, gz;

        gyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);  // read value

        stackX += ax;
        stackY += ay;                                   // stack value

        delay(10);                                      //sample delay
    }

    calc_x = stackX/CALC_SAMPLES;
    calc_y = stackY/CALC_SAMPLES;                 // calculate average
    
    return;
}

void DGP_Gyro::calibration(){                           // calibration and set cali_x, calc_y
    int16_t stackX = 0;
    int16_t stackY = 0;

    for(uint8_t i = 0; i < CALI_SAMPLES; i++){
        refresh();                                      // refresh calc_x, calc_y
        stackX += calc_x;
        stackY += calc_y;
    }
    cali_x = stackX/CALI_SAMPLES;
    cali_y = stackY/CALI_SAMPLES;                       // make calibrate value

    thhold_x = cali_x * thRef;
    thhold_y = cali_y * thRef;                          // set threshold

    isCompCali = true;

    return;
}

void DGP_Gyro::disableCali(){
    isCompCali = false;

    return;
}

boolean DGP_Gyro::compareValue(){
    refresh();
    if (calc_x < thhold_x || calc_y < thhold_y) {   // good state
        return true;
    } 
    else {
        return false;
    }
}


#endif
