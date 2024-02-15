#include "BME280.h"
#include <string.h> // Used for memcpy

/// @brief Initialize communications for the BME280 sensor
/// @return True if successful, false if not
bool BME280::init(void) {
    reset();

    bool successful = true;
    successful &= checkConnected(); // If it is connected then it is successful
    sleep_us(250);
    successful &= fetchCompensationData(); // If fetching worked then successful stays true, otherwise it's set to false
    sleep_us(250); // Wait a bit to allow the sensor to finish processing

    uint8_t buffer[3];
    readRegister(BME280_ctrl_hum, buffer, sizeof(buffer));
    buffer[0] = buffer[0] | 0b00000101; // Here we want to check the oversampling on ctrl_hum, binary 00000101
    buffer[2] = buffer[2] | 0b10110111; // Here we want to check the oversampling for ctrl_meas (press and temp) and setting mode
    writeRegister(BME280_ctrl_hum, buffer[0]); // Here we need to write on ctl_hum before writing on ctrl_meas
    writeRegister(BME280_ctrl_meas, buffer[2]);
    return successful;
}

/// @brief Soft reset the BME280 sensor
void BME280::reset(void){
    writeRegister(BME280_reset, 0xB6);
    sleep_ms(10);
}
/// @brief Check to see if it is connected
/// @param  
/// @return the right chip id, otherwise return false
bool BME280::checkConnected(void){
    uint8_t buffer[1];
    if(!readRegister(BME280_id, buffer)) return false;
    return buffer[0] == BME_chip_id;
}

/// @brief Read temperature, pressure and relative humidity from the BME280
/// @return True if successful, false if not
bool BME280::read() {
    uint8_t buffer[8];

    if(!readRegister(BME280_press_msb, buffer, sizeof(buffer))) return false;

    int32_t raw_t, raw_p, raw_h;
    /* We have 8 bits in each register, temp and press both have xlsb that have 0 in the bits 3-0
    *  We shift the bits around, we have 32 int in each register, so we shift it around so the bits fit
       at the right space. */
    raw_p = ((int32_t)buffer[0]<<12 | (int32_t)buffer[1]<<4 | (int32_t)buffer[2]>>4);
    raw_t = ((int32_t)buffer[3]<<12 | (int32_t)buffer[4]<<4 | (int32_t)buffer[5]>>4);
    raw_h = ((int32_t)buffer[6]<<8 | (int32_t)buffer[7]);

    compensateValues(&temperature, &pressure, &humidity, raw_t, raw_p, raw_h);

    return true;
}


/********** Private methods **********/

/// @brief write on the register
/// @param reg 
///        register, 2 bytes
/// @param data 
///        putting data to the register, 2 bytes
/// @return  True if successful, otherwise return false
bool BME280::writeRegister(uint8_t reg, uint8_t data){
    uint8_t buffer[2]{0};
    buffer[0] = reg;
    buffer[1] = data;
    
    return (i2c_write_timeout_us(_i2c, BME280_ADDRESS, buffer, 2, false, 10000) == 2);
}

/// @brief read from the register
/// @param reg 
///        register, 2 bytes
/// @param data 
///        data on the register
/// @param length 
///        length of the register
/// @return true if successful, otherwise return false
bool BME280::readRegister(uint8_t reg, uint8_t* data, uint8_t length){
    uint8_t buffer[1];
    buffer[0] = reg;
    if(i2c_write_timeout_us(_i2c, BME280_ADDRESS, buffer, 1, false, 10000) != 1) return false;

    return (i2c_read_timeout_us(_i2c, BME280_ADDRESS, data, length, false, 10000) == length);
}



/// @brief Type pun a uint16_t variable to an int16_t variable
/// @param x Input value in uint16_t format
/// @return A reinterpreted value containing the original data in int16_t format
[[nodiscard]] int16_t uint16_t_to_int16_t(uint16_t x) noexcept {
    return *reinterpret_cast<int16_t*>(&x);
}

/// @brief Type pun a uint8_t variable to an int8_t variable
/// @param x Input value in uint8_t format
/// @return A reinterpreted value containing the original data in int8_t format
[[nodiscard]] int8_t uint8_t_to_int8_t(uint8_t x) noexcept {
    return *reinterpret_cast<int8_t*>(&x);
}

/// @brief Fetch the compensation data from the BME280
/// @return True of fetching successful, false if not
bool BME280::fetchCompensationData(void) {
    // We want to read the full compensation register data
    uint8_t buffer[33]{0};

    // Two reads because the calibration/compensation registers are not aligned
    uint8_t compensation_reg_first = 0x88;
    uint8_t compensation_reg_second = 0xE1;
    if (i2c_write_timeout_us(_i2c, BME280_ADDRESS, &compensation_reg_first, 1, false, 10000) != 1) return false;
    if (i2c_read_timeout_us(_i2c, BME280_ADDRESS, buffer, 26, false, 10000) != 26) return false;

    if (i2c_write_timeout_us(_i2c, BME280_ADDRESS, &compensation_reg_second, 1, false, 10000) != 1) return false;
    if (i2c_read_timeout_us(_i2c, BME280_ADDRESS, buffer + 26, 7, false, 10000) != 7) return false;

    // Zero-initialize the comp_coeffs struct
    memset(&comp_coeffs, 0, sizeof(comp_coeffs));

    // Insert the compensation data into the struct for it
    comp_coeffs.dig_T1 = ((uint16_t)buffer[1] << 8) | (uint16_t)buffer[0];
    comp_coeffs.dig_T2 = uint16_t_to_int16_t(((uint16_t)buffer[3] << 8) | (uint16_t)buffer[2]);
    comp_coeffs.dig_T3 = uint16_t_to_int16_t(((uint16_t)buffer[5] << 8) | (uint16_t)buffer[4]);
    comp_coeffs.dig_P1 = ((uint16_t)buffer[7] << 8) | (uint16_t)buffer[6];
    comp_coeffs.dig_P2 = uint16_t_to_int16_t(((uint16_t)buffer[9] << 8) | (uint16_t)buffer[8]);
    comp_coeffs.dig_P3 = uint16_t_to_int16_t(((uint16_t)buffer[11] << 8) | (uint16_t)buffer[10]);
    comp_coeffs.dig_P4 = uint16_t_to_int16_t(((uint16_t)buffer[13] << 8) | (uint16_t)buffer[12]);
    comp_coeffs.dig_P5 = uint16_t_to_int16_t(((uint16_t)buffer[15] << 8) | (uint16_t)buffer[14]);
    comp_coeffs.dig_P6 = uint16_t_to_int16_t(((uint16_t)buffer[17] << 8) | (uint16_t)buffer[16]);
    comp_coeffs.dig_P7 = uint16_t_to_int16_t(((uint16_t)buffer[19] << 8) | (uint16_t)buffer[18]);
    comp_coeffs.dig_P8 = uint16_t_to_int16_t(((uint16_t)buffer[21] << 8) | (uint16_t)buffer[20]);
    comp_coeffs.dig_P9 = uint16_t_to_int16_t(((uint16_t)buffer[23] << 8) | (uint16_t)buffer[22]);
    comp_coeffs.dig_H1 = buffer[25];
    comp_coeffs.dig_H2 = uint16_t_to_int16_t(((uint16_t)buffer[27] << 8) | (uint16_t)buffer[26]);
    comp_coeffs.dig_H3 = buffer[28];
    comp_coeffs.dig_H4 = uint16_t_to_int16_t(((uint16_t)buffer[29] << 4) | (uint16_t)buffer[30] & 0x0F);
    comp_coeffs.dig_H5 = uint16_t_to_int16_t((((uint16_t)buffer[30] & 0xF0 ) >> 4) | ((uint16_t)buffer[31] << 4));
    comp_coeffs.dig_H6 = uint8_t_to_int8_t(buffer[32]);
    return true;
}

/// @brief Do the compensation calculation for temperature, pressure and humidity.
/// Note that pressure and humidity rely on temperature to stay accurate.
/// @param temperature Pointer to a float where the temperature value will be inserted (in °C)
/// @param pressure Pointer to a float where the pressure value will be inserted (in hPa)
/// @param humidity Pointer to a float where the humidity value will be inserted (in %RH)
/// @param raw_temperature Raw temperature reading from BME280 sensor
/// @param raw_pressure Raw pressure reading from BME280 sensor
/// @param raw_humidity Raw humidity reading from BME280 sensor
void BME280::compensateValues(  float* temperature,
                                float* pressure,
                                float* humidity,
                                int32_t raw_temperature,
                                int32_t raw_pressure,
                                int32_t raw_humidity) {
    // Temperature compensation
    int32_t t_fine;
    int32_t var1, var2, T;
    var1 = ((((raw_temperature >> 3) - ((int32_t)comp_coeffs.dig_T1 << 1))) * ((int32_t)comp_coeffs.dig_T2)) >> 11;
    var2 = (((((raw_temperature >> 4) - ((int32_t)comp_coeffs.dig_T1)) * ((raw_temperature >> 4) - ((int32_t)comp_coeffs.dig_T1))) >> 12) * ((int32_t)comp_coeffs.dig_T3)) >> 14;
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    *temperature = (float)T / 100.0f;

    // Pressure compensation
    int64_t p;
    var1 = 0;
    var2 = 0;
    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)comp_coeffs.dig_P6;
    var2 = var2 + ((var1 * (int64_t)comp_coeffs.dig_P5) << 17);
    var2 = var2 + (((int64_t)comp_coeffs.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)comp_coeffs.dig_P3) >> 8) + ((var1 * (int64_t)comp_coeffs.dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)comp_coeffs.dig_P1) >> 33;
    if (var1 == 0) {
        *pressure = 0.0f;
    } else {
        p = 1048576 - raw_pressure;
        p = (((p << 31) - var2) * 3125) / var1;
        var1 = (((int64_t)comp_coeffs.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
        var2 = (((int64_t)comp_coeffs.dig_P8) * p) >> 19;
        p = ((p + var1 + var2) >> 8) + (((int64_t)comp_coeffs.dig_P7) << 4);
        *pressure = (float)((uint32_t)p) / 25600.0f;
    }

    // Humidity compensation
    int32_t h_temp;
    h_temp = (t_fine - ((int32_t)76800));
    h_temp = (((((raw_humidity << 14) - (((int32_t)comp_coeffs.dig_H4) << 20) - (((int32_t)comp_coeffs.dig_H5) * h_temp)) + ((int32_t)16384)) >> 15) * (((((((h_temp * ((int32_t)comp_coeffs.dig_H6)) >> 10) * (((h_temp * ((int32_t)comp_coeffs.dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) * ((int32_t)comp_coeffs.dig_H2) + 8192) >> 14));
    h_temp = (h_temp - (((((h_temp >> 15) * (h_temp >> 15)) >> 7) * ((int32_t)comp_coeffs.dig_H1)) >> 4));
    h_temp = (h_temp < 0 ? 0 : h_temp);
    h_temp = (h_temp > 419430400 ? 419430400 : h_temp);
    *humidity = (float)((uint32_t)(h_temp >> 12)) / 1024.0f;
}