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

/// @brief 
/// @param  
/// @return  
bool LMP91::init(void){
    reset();
//    write_register();
    sleep_ms(30);
    return true;
}

/// @brief check 
/// @param status 
/// @return 
bool LMP91::set_STATUS(STATUS status){
    uint8_t bits = read_register(LMP91_STATUS);
    bits = (bits & 0b11111110) | ((uint8_t)status & 0b00000001);
    write_register(LMP91_STATUS, bits);
    return true;
}

/// @brief 
/// @param lock 
/// @return 
bool LMP91::set_LOCK(LOCK lock){
    uint8_t bits = read_register(LMP91_STATUS);
    bits = (bits & 0b11111110) | ((uint8_t)lock & 0b00000001);
    write_register(LMP91_LOCK, bits);
    return true;
}

/// @brief 
/// @param gain 
/// @return 
bool LMP91::set_TIA_GAIN(TIA_GAIN gain){
    uint8_t bits = read_register(LMP91_TIACN);
    bits  = (bits & 0b11100011) | (((uint8_t)gain << 2) & 0b00011100);
    write_register(LMP91_TIACN, bits);
    return true;
}

/// @brief 
/// @param load 
/// @return 
bool LMP91::set_R_LOAD(R_LOAD load){
    uint8_t bits = read_register(LMP91_TIACN);
    bits = (bits & 0b11111100) | ((uint8_t)load & 0b00000011);
    write_register(LMP91_TIACN, bits);
    return true;
}

/// @brief 
/// @param source 
/// @return 
bool LMP91::set_REF_SOURCE(REF_SOURCE source){
    uint8_t bits = read_register(LMP91_REFCN);
    bits = (bits & 0b01111111) | (((uint8_t)source << 7) & 0b10000000);
    write_register(LMP91_REFCN, bits);
    return true;
}

/// @brief 
/// @param internal 
/// @return 
bool LMP91::set_INT_Z(INT_Z internal){
    uint8_t bits = read_register(LMP91_REFCN);
    bits = (bits & 0b01100000) | (((uint8_t)internal << 5) & 0b10011111);
    write_register(LMP91_REFCN, bits);
    return true;
}

/// @brief 
/// @param signal 
/// @return 
bool LMP91::set_BIAS_SIGN(BIAS_SIGN signal){
    uint8_t bits = read_register(LMP91_REFCN);
    bits = (bits & 0b00010000) | (((uint8_t)signal << 4) & 0b11101111);
    write_register(LMP91_REFCN, bits);
    return true;
}

/// @brief 
/// @param bias 
/// @return 
bool LMP91::set_BIAS(BIAS bias){
    uint8_t bits = read_register(LMP91_REFCN);
    bits = (bits & 0b00001111) | ((uint8_t)bias& 0b11110000);
    write_register(LMP91_REFCN, bits);
    return true;
}

/// @brief 
/// @param shorting 
/// @return 
bool LMP91::set_FET_SHORT(FET_SHORT shorting){
    uint8_t bits = read_register(LMP91_MODE);
    bits = (bits & 0b01111111) | (((uint8_t)shorting << 7) & 0b1111111);
    write_register(LMP91_MODE, bits);
    return true;
}

/// @brief 
/// @param mode 
/// @return 
bool LMP91::set_OP_MODE(OP_MODE mode){
    uint8_t bits = read_register(LMP91_MODE);
    bits = (bits & 0b11111000) | ((uint8_t)mode & 0b00000111);
    write_register(LMP91_MODE, bits);
    return true;
}

/// @brief 
/// @param reg 
/// @param data 
/// @return 
bool LMP91::write_register(uint8_t reg, uint8_t data){
    uint8_t buffer[2] {0};

    buffer[0] = reg;
    return (i2c_write_timeout_us(_i2c, LMP91_ADDRESS, buffer, 1, false, 10000) == 1);
}

/// @brief 
/// @param reg_address 
/// @return 
uint8_t LMP91::read_register(uint8_t reg_address){
    uint8_t buffer[1];

    buffer[0] = reg_address;

    i2c_write_timeout_us(_i2c, LMP91_ADDRESS, buffer, 1, true, 10000);
    //sleep_ms(1); // Ef ekki virka unkommenta
    i2c_read_timeout_us(_i2c, LMP91_ADDRESS, buffer, 1, false, 10000);
    return (uint8_t)(buffer[0]);
}