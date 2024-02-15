/*
 *  Title: SEN55.cpp
 *  Description:
 *  Author: Tinna Osk Traustadottir
 */
#include "SEN55.h"
#include <string.h>
#include <hardware/i2c.h>
#include <stdio.h>

/// @brief 
/// @param data 
/// @return
uint8_t CalcCrc(uint8_t data[2]) {
    uint8_t crc = 0xFF;
    for(int i = 0; i < 2; i++) {
        crc ^= data[i];
        for(uint8_t bit = 8; bit > 0; --bit) {
            if(crc & 0x80) {
            crc = (crc << 1) ^ 0x31u;
            } else {
                 crc = (crc << 1);
             }
         }
      }
      return crc;
}

bool SEN55::init(void){
    reset();
    sendCommand(SEN55_START_MEAS);
    sleep_ms(30);
}

/// @brief Reset the SEN55 sensor
/// @param  
void SEN55::reset(void) {
    sendCommand(SEN55_RESET);
    sleep_ms(30);
}

/// @brief Ask the sensor if new data is ready to be read
/// @param  
/// @return True id data is ready to be read, false if not
bool SEN55::dataReady(void) {
    return(readRegister(SEN55_READ_DATA_READY) == 1);
}

bool SEN55::read(SEN55_VALUES* values) {
    uint8_t buffer[24]{0};

    buffer[0] = (SEN55_READ_MEAS_VALUES & 0xFF00) >> 8;
    buffer[1] = SEN55_READ_MEAS_VALUES & 0x00FF;

    i2c_write_timeout_us(_i2c, SEN55_ADDRESS, buffer, 2, false, 10000);
    
    sleep_ms(20); // Delay between write and read specified by the datasheet as 10 ms

    i2c_read_timeout_us(_i2c, SEN55_ADDRESS, buffer, 24, false, 10000);

    for(uint16_t i = 0; i < 24; i += 3) {
        if(CalcCrc(buffer + i) != buffer[i+2]) {
            return false;
        }
    }
    
    uint16_t _pm1 = 0, _pm2_5 = 0, _pm4 = 0, _pm10 = 0;
    int16_t _rh = 0, _temp = 0, _voc = 0, _nox = 0;

    _pm1 |= buffer[0];
    _pm1 <<= 8;
    _pm1 |= buffer[1];

    _pm2_5 |= buffer[3];
    _pm2_5 <<= 8;
    _pm2_5 |= buffer[4];

    _pm4 |= buffer[6];
    _pm4 <<= 8;
    _pm4 |= buffer[7];

    _pm10 |= buffer[9];
    _pm10 <<= 8;
    _pm10 |= buffer[10];

    _rh |= buffer[12];
    _rh <<= 8;
    _rh |= buffer[13];

    _temp |= buffer[15];
    _temp <<= 8;
    _temp |= buffer[16];

    _voc |= buffer[18];
    _voc <<= 8;
    _voc |= buffer[19];

    _nox |= buffer[21];
    _nox <<= 8;
    _nox |= buffer[22];

    (*values).pm1 = _pm1/10.0f;
    (*values).pm2_5 = _pm2_5/10.0f;
    (*values).pm4 = _pm4/10.0f;
    (*values).pm10 = _pm10/10.0f;
    (*values).RH = _rh/100.0f;
    (*values).temp = _temp/200.0f;
    (*values).VOC = _voc/10.0f;
    (*values).NOx = _nox/10.0f;

    return true;
}

bool sendCommand(uint16_t command){
    uint16_t buffer[2]{0};

    buffer[0] = (command >> 8) & 0xFF00;
    buffer[1] = command & 0x00FF;
    return (i2c_write_timeout_us(_i2c, SEN55_ADDRESS, buffer, 2, false, 10000) == 2);
}
uint16_t readRegister(uint16_t reg_address){
    uint16_t buffer[2];
    
    buffer[0] = (reg_address >> 8) & 0xFF00;
    buffer[1] = reg_address & 0x00FF;

    i2c_write_timeout_us(_i2c, SEN55_ADDRESS, buffer, 2, false, 10000);
    sleep_ms(10);
    i2c_read_timeout_us(_i2c, SEN55_ADDRESS, buffer, 2, false, 10000);

    return (uint16_t)(buffer[0] << 8 | (buffer[1] & 0xFF));
}