#pragma once
#include <hardware/i2c.h>

const uint8_t BME280_default_I2Caddr = 0x76;
const uint8_t BME280_alt_I2Caddr = 0x77;
const uint8_t BME_chip_id = 0x60;

const uint8_t BME280_hum_lsb = 0xFE;
const uint8_t BME280_hum_msb = 0xFD;
const uint8_t BME280_temp_xlsb = 0xFC;
const uint8_t BME280_temp_lsb = 0xFB;
const uint8_t BME280_temp_msb = 0xFA;
const uint8_t BME280_press_xlsb = 0xF9;
const uint8_t BME280_press_lsb = 0xF8;
const uint8_t BME280_press_msb = 0xF7;
const uint8_t BME280_config = 0xF5;
const uint8_t BME280_ctrl_meas = 0xF4;
const uint8_t BME280_status = 0xF3;
const uint8_t BME280_ctrl_hum = 0xF2;
const uint8_t BME280_calib26 = 0xE1;
const uint8_t BME280_reset = 0xE0;
const uint8_t BME280_id = 0xD0;
const uint8_t BME280_calib00 = 0x88;

class BME280 {
public: 
    BME280(i2c_inst_t* i2c, uint8_t addr = BME280_default_I2Caddr) : _i2c(i2c), BME280_ADDRESS(addr) {};
    BME280() {};

    bool init(void);
    void reset(void);
    bool read(void);
    bool checkConnected(void);

    float temperature, pressure, humidity;

private:
    i2c_inst_t* _i2c;
    uint8_t BME280_ADDRESS;

    /// @brief We need this to get proper values
    struct BME_Comp_Coeff_t {
      uint16_t dig_T1, dig_P1;
        int16_t dig_T2, dig_T3, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9, dig_H2, dig_H4, dig_H5;
        uint8_t dig_H1, dig_H3;
        int8_t dig_H6;  
    } comp_coeffs;
    bool fetchCompensationData(void);
    void compensateValues(  float* temperature,
                            float* pressure,
                            float* humidity,
                            int32_t raw_temperature,
                            int32_t raw_pressure,
                            int32_t raw_humidity);

    bool readRegister(uint8_t reg, uint8_t* data, uint8_t length = 1);
    bool writeRegister(uint8_t reg, uint8_t data);
};