/*
 *  Title: LMP91.h
 *  Description:
 *  Author: Tinna Osk Traustadottir
 */
#pragma once
#include <hardware/i2c.h>
#include "LMP91_regs.h"

class LMP91 {
public:
    LMP91(i2c_inst_t* i2c, uint8_t addr = LMP91_DEFAULT_I2CADDR);
    bool init(void);

    void reset(void);

    bool set_STATUS(STATUS status);
    bool set_LOCK(LOCK lock);
    bool set_TIA_GAIN(TIA_GAIN gain);
    bool set_R_LOAD(R_LOAD load);
    bool set_REF_SOURCE(REF_SOURCE source);
    bool set_INT_Z(INT_Z internal);
    bool set_BIAS_SIGN(BIAS_SIGN signal);
    bool set_BIAS(BIAS bias);
    bool set_FET_SHORT(FET_SHORT shorting);
    bool set_OP_MODE(OP_MODE mode);

private:
    i2c_inst_t* _i2c;
    uint8_t LMP91_ADDRESS;

    bool write_register(uint8_t reg, uint8_t data);
    uint8_t read_register(uint8_t reg_address);
};
