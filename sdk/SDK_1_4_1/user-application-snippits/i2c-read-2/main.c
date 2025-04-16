
/* (c) SynchronicIT B.V. 2021. All rights reserved.                                   05/07/2024
             _____                  _               ______         _             
            / ____|                | |             |  ____|       (_)             TM
           | (___  _   _ _ __   ___| |__  _ __ ___ | |__ _   _ ___ _  ___  _ __  
            \___ \| | | | '_ \ / __| '_ \| '__/ _ \|  __| | | / __| |/ _ \| '_ \ 
            ____) | |_| | | | | (__| | | | | | (_) | |  | |_| \__ \ | (_) | | | |
           |_____/ \__, |_| |_|\___|_| |_|_|  \___/|_|   \__,_|___/_|\___/|_| |_|
                    __/ |                                                        
                   |___/                                 http://www.synchronicit.nl/ 

  This software is confidential and proprietary of SynchronicIT and is subject to the terms and 
  conditions defined in file 'LICENSE.txt', which is part of this source code package. You shall 
  not disclose such Confidential Information and shall use it only in accordance with the terms 
  of the license agreement.

                                                                  USER APPLICATION version 1.4.1
*/

#include "user-application.h"

#define BMP280_ADDR (0x76)
// #define BMP280_ADDR (0x77)

// Registers available on the sensor:
enum {
BMP280_REGISTER_DIG_T1 = 0x88,
BMP280_REGISTER_DIG_T2 = 0x8A,
BMP280_REGISTER_DIG_T3 = 0x8C,
BMP280_REGISTER_DIG_P1 = 0x8E,
BMP280_REGISTER_DIG_P2 = 0x90,
BMP280_REGISTER_DIG_P3 = 0x92,
BMP280_REGISTER_DIG_P4 = 0x94,
BMP280_REGISTER_DIG_P5 = 0x96,
BMP280_REGISTER_DIG_P6 = 0x98,
BMP280_REGISTER_DIG_P7 = 0x9A,
BMP280_REGISTER_DIG_P8 = 0x9C,
BMP280_REGISTER_DIG_P9 = 0x9E,
BMP280_REGISTER_CHIPID = 0xD0,
BMP280_REGISTER_VERSION = 0xD1,
BMP280_REGISTER_SOFTRESET = 0xE0,
BMP280_REGISTER_CAL26 = 0xE1,
BMP280_REGISTER_STATUS = 0xF3,
BMP280_REGISTER_CONTROL = 0xF4,
BMP280_REGISTER_CONFIG = 0xF5,
BMP280_REGISTER_PRESSUREDATA = 0xF7,
BMP280_REGISTER_TEMPDATA = 0xFA,
};
/*!
*  Struct to hold calibration data.
*/
typedef struct {
uint16_t dig_T1; // dig_T1 cal register
int16_t dig_T2;  //  dig_T2 cal register
int16_t dig_T3;  // dig_T3 cal register
uint16_t dig_P1; // dig_P1 cal register
int16_t dig_P2;  // dig_P2 cal register
int16_t dig_P3;  // dig_P3 cal register
int16_t dig_P4;  // dig_P4 cal register
int16_t dig_P5;  // dig_P5 cal register
int16_t dig_P6;  // dig_P6 cal register
int16_t dig_P7;  // dig_P7 cal register
int16_t dig_P8;  // dig_P8 cal register
int16_t dig_P9;  // dig_P9 cal register
} bmp280_calib_data;



static bmp280_calib_data calib;


void setup() {

    Serial.begin(115200);
    I2C.begin(D2, D1);

    // read calibration data
    uint8_t buffer[] = { BMP280_REGISTER_DIG_T1, 0 };
    I2C.write(BMP280_ADDR, buffer, 1, true);
    I2C.read(BMP280_ADDR, (uint8_t*)&calib, sizeof(calib), true);

    // set sampling
    buffer[0] = BMP280_REGISTER_CONFIG;
    uint8_t mode = 3;
    uint8_t tempSampling = 5;
    uint8_t pressSampling = 5;
    uint8_t filter = 0;
    uint8_t duration = 0;

    buffer[1] = (duration << 5) | (filter << 2) | 0;
    I2C.write(BMP280_ADDR, buffer, 2, true);

    buffer[0] = BMP280_REGISTER_CONTROL;
    buffer[1] = (tempSampling << 5) | (pressSampling << 2) | mode;
    I2C.write(BMP280_ADDR, buffer, 2, true);

}

int32_t correctTemperature(int32_t adc_T) {
    return ((
        (((((adc_T >>7) - ((int32_t)calib.dig_T1 << 1)))
        * ((int32_t)calib.dig_T2)) >>11) +  
        ((((((adc_T >>8) - ((int32_t)calib.dig_T1)) * 
            ((adc_T >>8) - ((int32_t)calib.dig_T1))) >>12) *  
            ((int32_t)calib.dig_T3)) >>14)
    ) * 5 + 128) >>8;
}

void loop() {
    // read temp
    uint8_t buffer[] = { BMP280_REGISTER_TEMPDATA, 0, 0, 0 };
    int32_t adc_T = 0;
    I2C.write(BMP280_ADDR, buffer, 1, true);
    I2C.read(BMP280_ADDR, buffer, 3, true);
    adc_T = buffer[0] << 16 | buffer[1] << 8 | buffer[2];
    // correct reading
    int32_t tmp = correctTemperature(adc_T);
    // print reading
    Serial.print("Temp = %i.%02u deg\n", tmp/100,  tmp%100);
    delay(500);
}

