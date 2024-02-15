/*
 *  Title: LMP91.cpp
 *  Description:
 *  Author: Tinna Osk Traustadottir
 */

#include <string.h>
#include <stdio.h>
#include <hardware/i2c.h>
#include "LMP91.h"

/// @brief Construct a new LMP91::LMP91 object
/// @param i2c 
///        The I2C hardware instance to be used
/// @param addr 
///        The I2C address for the sensor, defaulted to 0x90
LMP91::LMP91(i2c_inst_t *i2c, uint8_t addr){
    _i2c = i2c;
    LMP91_ADDRESS = addr;
}

bool LMP91::init(void){
    reset();
    sendCommand(LMP91_STATUS);
    sleep_ms(30);
}

//void LMP91::reset(void){
//    sendCommand()
//}