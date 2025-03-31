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
#define CALI_SAMPLES 30

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

        uint8_t outCount = 0;

        boolean isCompCali = false; //check for cali

        void refresh();             // re-calculate x, y
    public: 
        DGP_Gyro(){;}
        DGP_Gyro(uint8_t d, uint8_t c, float t = 0.15);
        ~DGP_Gyro(){;}

        void init(boolean dev = false);
        int16_t getX(){ return calc_x; }
        int16_t getY(){ return calc_y; }
        boolean getCali(){ return isCompCali; }
        void calibration(boolean dev = false);      // calibration and set cali_x, cali_y
        void disableCali();                         // disable calibration
        boolean compareValue(boolean dev = false);  // compare cali and calc
        void printSerialCali();
};

DGP_Gyro::DGP_Gyro(uint8_t d, uint8_t c, float t = 0.15){
    sda_pin = d;
    scl_pin = c;
    thRef = t;
}

void DGP_Gyro::init(boolean dev = false){
    Wire.begin();       // wire communication start
    gyro.initialize();  // gyro module init
    if(dev) Serial.println((gyro.testConnection())? 
                        "==== GYRO CONNECTED! ====":
                        "==== GYRO FAILED ====");


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

void DGP_Gyro::calibration(boolean dev = false){                           // calibration and set cali_x, calc_y
    int16_t stackX = 0;
    int16_t stackY = 0;

    gyro.setXGyroOffset(0);
    gyro.setYGyroOffset(0);

    if(dev) Serial.println("==== BEGIN CALI ====");

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

    if(dev) Serial.println("==== END OF CALI ====");

    return;
}

void DGP_Gyro::printSerialCali(){
    Serial.println("==== Cali ==== ");
    Serial.print("Cali X: "); Serial.print(cali_x); Serial.print(", Cali Y: "); Serial.println(cali_y); 
    Serial.print("TH X: "); Serial.print(thhold_x); Serial.print(", TH Y: "); Serial.println(thhold_y); 
    Serial.println("============= ");
}

void DGP_Gyro::disableCali(){
    isCompCali = false;

    return;
}

boolean DGP_Gyro::compareValue(boolean dev = false){                                       // check posture, if good = true
    refresh();

    if(dev){
        Serial.print("+cali_x:"); Serial.print(cali_x+thhold_x); Serial.print(",calc_x:"); Serial.print(calc_x); Serial.print(",-cali_x:"); Serial.println(cali_x-thhold_x);
        Serial.print("+cali_y:"); Serial.print(cali_y+thhold_y); Serial.print(",calc_y:"); Serial.print(calc_y); Serial.print(",-cali_y:"); Serial.println(cali_y-thhold_y);
    }
    if (calc_x > cali_x+thhold_x || calc_y > cali_y+thhold_y
        || calc_x < cali_x-thhold_x || calc_y < cali_y-thhold_y) {         // bad state
            outCount++; 
    }
    else {
        outCount = 0;
        return true;
    }

    if(outCount > 10){
        outCount = 0;
        return false;
    }

    return true;
}


#endif
