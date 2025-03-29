#ifndef _DGP_GYRO_H   // header-guard
#define _DGP_GYRO_H

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

#endif
