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
//    write_register();
    sleep_ms(30);
    return true;
}

/// @brief Check if on or off
/// @param status 
/// @return true
bool LMP91::set_STATUS(STATUS status){
    uint8_t bits = read_register(LMP91_STATUS);
    bits = (bits & 0b11111110) | ((uint8_t)status & 0b00000001);
    write_register(LMP91_STATUS, bits);
    return true;
}

/// @brief Enables and disables writing of the TIACN and REFCN
///        registers
/// @param lock The lock chosen, (see LOCK)
/// @return true
bool LMP91::set_LOCK(LOCK lock){
    uint8_t bits = read_register(LMP91_STATUS);
    bits = (bits & 0b11111110) | ((uint8_t)lock & 0b00000001);
    write_register(LMP91_LOCK, bits);
    return true;
}

/// @brief Transimpedance Gain
/// @param gain The gain chosen (see TIA_GAIN) 
/// @return true
bool LMP91::set_TIA_GAIN(TIA_GAIN gain){
    uint8_t bits = read_register(LMP91_TIACN);
    bits  = (bits & 0b11100011) | (((uint8_t)gain << 2) & 0b00011100);
    write_register(LMP91_TIACN, bits);
    return true;
}

/// @brief Load resistance
/// @param load The load chosen (see R_LOAD)
/// @return true
bool LMP91::set_R_LOAD(R_LOAD load){
    uint8_t bits = read_register(LMP91_TIACN);
    bits = (bits & 0b11111100) | ((uint8_t)load & 0b00000011);
    write_register(LMP91_TIACN, bits);
    return true;
}

/// @brief Reference voltage source 
/// @param source the voltage source chosen, (see REF_SOURCE)
/// @return true
bool LMP91::set_REF_SOURCE(REF_SOURCE source){
    uint8_t bits = read_register(LMP91_REFCN);
    bits = (bits & 0b01111111) | (((uint8_t)source << 7) & 0b10000000);
    write_register(LMP91_REFCN, bits);
    return true;
}

/// @brief Internal zero selection (percentage of the source reference)
/// @param internal the internal zero chosen (see INT_Z)
/// @return true
bool LMP91::set_INT_Z(INT_Z internal){
    uint8_t bits = read_register(LMP91_REFCN);
    bits = (bits & 0b01100000) | (((uint8_t)internal << 5) & 0b10011111);
    write_register(LMP91_REFCN, bits);
    return true;
}

/// @brief Selection of the bias polarity
/// @param signal The signal chosen (see BIAL_SIGN)
/// @return True
bool LMP91::set_BIAS_SIGN(BIAS_SIGN signal){
    uint8_t bits = read_register(LMP91_REFCN);
    bits = (bits & 0b00010000) | (((uint8_t)signal << 4) & 0b11101111);
    write_register(LMP91_REFCN, bits);
    return true;
}

/// @brief Bias selection (Percentage of the source reference)
/// @param bias The bias chosen (see BIAS)
/// @return True
bool LMP91::set_BIAS(BIAS bias){
    uint8_t bits = read_register(LMP91_REFCN);
    bits = (bits & 0b00001111) | ((uint8_t)bias& 0b11110000);
    write_register(LMP91_REFCN, bits);
    return true;
}

/// @brief Shorting FET feature
/// @param shorting the shorting chosen (see FET_SHORT)
/// @return True
bool LMP91::set_FET_SHORT(FET_SHORT shorting){
    uint8_t bits = read_register(LMP91_MODE);
    bits = (bits & 0b01111111) | (((uint8_t)shorting << 7) & 0b1111111);
    write_register(LMP91_MODE, bits);
    return true;
}

/// @brief Mode of operation selection
/// @param mode The mode chosen (see OP_MODE)
/// @return True
bool LMP91::set_OP_MODE(OP_MODE mode){
    uint8_t bits = read_register(LMP91_MODE);
    bits = (bits & 0b11111000) | ((uint8_t)mode & 0b00000111);
    write_register(LMP91_MODE, bits);
    return true;
}

/// @brief Write to the register
/// @param reg  register
/// @param data  data written
/// @return the data written to the register?
bool LMP91::write_register(uint8_t reg, uint8_t data){
    uint8_t buffer[2] {0};

    buffer[0] = reg;
    return (i2c_write_timeout_us(_i2c, LMP91_ADDRESS, buffer, 1, false, 10000) == 1);
}

/// @brief Read the register address
/// @param reg_address 
/// @return buffer?
uint8_t LMP91::read_register(uint8_t reg_address){
    uint8_t buffer[1];

    buffer[0] = reg_address;

    i2c_write_timeout_us(_i2c, LMP91_ADDRESS, buffer, 1, true, 10000);
    //sleep_ms(1); // Ef ekki virka unkommenta
    i2c_read_timeout_us(_i2c, LMP91_ADDRESS, buffer, 1, false, 10000);
    return (uint8_t)(buffer[0]);
}