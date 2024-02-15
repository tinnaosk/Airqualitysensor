/*
 *  Title: SEN55.h
 *  Description:
 *  Author: Tinna Osk Traustadottir
 */
#pragma once
#include <hardware/i2c.h>

const uint16_t SEN55_DEFAULT_I2CADDR = 0x69;    

const uint16_t SEN55_START_MEAS = 0x0021;           // Starts measurement,
const uint16_t SEN55_START_MEAS_RHT = 0x0037;       // Starts a continous measurement without PM, only humidity, temperature, VOC and NOx
const uint16_t SEN55_STOP_MEAS = 0x0104;            // Stops measurement, return to initial stage
const uint16_t SEN55_READ_DATA_READY = 0x0202;      // Find out when new meas is available
const uint16_t SEN55_READ_MEAS_VALUES = 0x03C4;     // Reads meas values and resets data ready flag
const uint16_t SEN55_RW_TEMP_COMP_PARAM = 0x60B2;   // Compensate temperature
const uint16_t SEN55_RW_WARM_START_PARAM = 0x60C2;  // Optimized for cold start by default
const uint16_t SEN55_RW_VOC_TUNING_PARAM = 0x60D0;  // VOC algorithm tuning parameters
const uint16_t SEN55_RW_NOx_TUNING_PARAM = 0x60E1;  // VOx algorithm tuning parameters
const uint16_t SEN55_RW_RHT_ACC_MODE = 0x60F7;      // Read/write RH/T acceleration mode
const uint16_t SEN55_RW_VOC_STATE = 0x6181;         // Read/write VOC algorithm state
const uint16_t SEN55_START_FAN_CLEANING = 0x5607;   // Starts the fan cleaning manually
const uint16_t SEN55_RW_AUTO_CLEAN_INTER = 0x8004;  // Read/writes the interval of the periodic fan cleaning
const uint16_t SEN55_READ_PRODUCT_NAME = 0xD014;    // Returns the product name
const uint16_t SEN55_READ_SERIAL = 0xD033;          // Returns the serial number
const uint16_t SEN55_READ_FIRMWARE = 0xD100;        // Returns the firmware version
const uint16_t SEN55_READ_DEVICE_STATUS = 0xD206;   // Reads the device status register
const uint16_t SEN55_CLEAR_DEVICE_STATUS = 0xD210;  // Clears all flags in device status register
const uint16_t SEN55_RESET = 0xD304;                // Software reset cmd

struct SEN55_VALUES {
    float pm1;
    float pm2_5;
    float pm4;
    float pm10;
    float RH;
    float temp;
    float NOx;
    float VOC;
};

class SEN55 {
public:
    SEN55(i2c_inst_t *i2c, uint16_t addr = SEN55_DEFAULT_I2CADDR);
    bool init(void);

    void reset(void);
    bool dataReady(void);
    bool SEN55::read(SEN55_VALUES* values);


private:
    i2c_inst_t* _i2c;
    uint16_t SEN55_ADDRESS;
    
    bool sendCommand(uint16_t command);
    uint16_t readRegister(uint16_t reg_address);
};