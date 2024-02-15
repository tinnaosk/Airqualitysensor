/*
 *  Title: LMP91.h
 *  Description:
 *  Author: Tinna Osk Traustadottir
 */
#pragma once
#include <hardware/i2c.h>

const uint8_t LMP91_DEFAULT_I2CADDR = 0x90;

const uint8_t LMP91_STATUS = 0x00;  // Power-on status 
const uint8_t LMP91_LOCK = 0x01;    // Enables and disables writing of the TIACN and REFCN
const uint8_t LMP91_TIACN = 0x10;   // Transimpedance gaind and the load resistance
const uint8_t LMP91_REFCN = 0x11;   // Reference control register
const uint8_t LMP91_MODE = 0x12;    // Mode register, configures the operation modes

class LMP91 {
public:
    LMP91(i2c_inst_t* i2c, uint8_t addr = LMP91_DEFAULT_I2CADDR);
    bool init(void);

    void reset(void);


private:
    i2c_inst_t* _i2c;
    uint8_t LMP91_ADDRESS;

    bool sendCommand(uint8_t command);
    uint8_t readRegister(uint8_t reg_address);
};
