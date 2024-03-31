/* 
 * Title:       RFM95W Library
 * Description: A software library for the RFM95W module based on the RadioHead library
 * Author:      
 */

#include "RFM95W.h"
#include "RFM95W_regs.h"
#include <hardware/spi.h>
#include <hardware/gpio.h>
#include <math.h>

/* TODO:
 *  - Add interrupts attachment to init funciton
 *  - Change setModemConfig to use better naming
*/

/// @brief Initialize the module using SPI. This function will block for at least 100ms.
void RFM95W::init() {
    // Initialize SPI
    spi_init(_spi, 1000000); // 1 Mbaud/s (datasheet max is 10 MHz SCK freq)
    gpio_set_dir(_cs_pin, GPIO_OUT);
    gpio_pull_up(_cs_pin);
    gpio_put(_cs_pin, true);
    sleep_ms(100);

    // TODO: Add interrupt handling

    // Set up FIFO
    writeRegister(RFM95W_REG::FIFO_TX_BASE_ADDR, 0);
    writeRegister(RFM95W_REG::FIFO_RX_BASE_ADDR, 0);

    // Set module to idle
    setMode(RFM95W_MODE::idle);

    // Set up default config
    setModemConfig(ModemConfigChoice::Bw125Cr45Sf128);
    setPreambleLength(8);
    setFrequency(868.0f);
    setTxPower(13);
}

/// @brief Set the working mode of the RFM95W module
/// @param mode The mode to set the modem to
void RFM95W::setMode(RFM95W_MODE mode) {
    uint8_t data;
    switch (mode)
    {
        case RFM95W_MODE::idle:     data = RFM95W_OP_MODE::STDBY;           break;
        case RFM95W_MODE::sleep:    data = RFM95W_OP_MODE::SLEEP;           break;
        case RFM95W_MODE::rx:       data = RFM95W_OP_MODE::RXCONTINUOUS;    break;
        case RFM95W_MODE::tx:       data = RFM95W_OP_MODE::TX;              break;
    
        default:                    data = RFM95W_OP_MODE::SLEEP;           break;
    }
    _mode = mode;

    writeRegister(RFM95W_REG::OP_MODE, data);

    if (mode == RFM95W_MODE::rx) writeRegister(RFM95W_REG::DIO_MAPPING_1, 0x00); // Interrupt on RX done
    if (mode == RFM95W_MODE::tx) writeRegister(RFM95W_REG::DIO_MAPPING_1, 0x40); // Interrupt on TX done
}

/// @brief Set the TX power of the module
/// @param power Power in dBm, can range from -1dBm to 23 dBm (range dependent on RFO pin usage)
/// @param useRFO Use the RF Out pin or PA_BOOST pin. RFM95W must use the PA_BOOST pin.
/// RFO enabled limits power between -1 and 14 dBm, RFO disabled limits power between 5 and 23 dBm.
void RFM95W::setTxPower(int8_t power, bool useRFO) {
    if (useRFO) {
        if (power > 14) power = 14;
        if (power < -1) power = -1;

        writeRegister(RFM95W_REG::PA_CONFIG, RFM95W_PA_CONFIG::MAX_POWER | (power + 1));
    } else {
        if (power > 23) power = 23;
        if (power < 5) power = 5;

        if (power > 20) {
            writeRegister(RFM95W_REG::PA_DAC, RFM95W_PA_DAC::ENABLE);
            power -= 3;
        } else {
            writeRegister(RFM95W_REG::PA_DAC, RFM95W_PA_DAC::DISABLE);
        }

        writeRegister(RFM95W_REG::PA_CONFIG, RFM95W_PA_CONFIG::PA_SELECT | (power-5));
    }
}

/// @brief Set the length of the preamble
/// @param bytes Preamble length in bytes
void RFM95W::setPreambleLength(uint16_t bytes) {
    writeRegister(RFM95W_REG::PREAMBLE_MSB, bytes >> 8);
    writeRegister(RFM95W_REG::PREAMBLE_LSB, bytes & 0xff);
}

/// @brief Set the center frequency of the module
/// @param center Center frequency in MHz
/// @return True if successful, false if unsuccessful
bool RFM95W::setFrequency(float center) {
    uint32_t frf = (center * 1000000.0f) / RFM95W_FSTEP;

    writeRegister(RFM95W_REG::FRF_MSB, (frf >> 16) & 0xff);
    writeRegister(RFM95W_REG::FRF_MID, (frf >> 8) & 0xff);
    writeRegister(RFM95W_REG::FRF_LSB, frf & 0xff);

    _usingHFport = (center >= 779.0f);

    return true;
}

/// @brief Enable or disable the temperature compensated crystal oscillator
/// @param enabled Enable or disable the TCXO
void RFM95W::enableTCXO(bool enabled) {
    while ((readRegister(RFM95W_REG::TCXO) & RFM95W_TCXO::INPUT_ON) != RFM95W_TCXO::INPUT_ON) {
        setMode(RFM95W_MODE::sleep);
        writeRegister(RFM95W_REG::TCXO, (readRegister(RFM95W_REG::TCXO) | RFM95W_TCXO::INPUT_ON));
    }
}

/// @brief Set the spreading factor of the LoRa comms
/// @param sf The spreading factor to set
void RFM95W::setSpreadingFactor(uint8_t sf) {
    switch (sf)
    {
        case 6:     sf = RFM95W_MODEM_CONFIG_2::SPREADING_FACTOR_64_CPS;    break;
        case 7:     sf = RFM95W_MODEM_CONFIG_2::SPREADING_FACTOR_128_CPS;   break;
        case 8:     sf = RFM95W_MODEM_CONFIG_2::SPREADING_FACTOR_256_CPS;   break;
        case 9:     sf = RFM95W_MODEM_CONFIG_2::SPREADING_FACTOR_512_CPS;   break;
        case 10:    sf = RFM95W_MODEM_CONFIG_2::SPREADING_FACTOR_1024_CPS;  break;
        case 11:    sf = RFM95W_MODEM_CONFIG_2::SPREADING_FACTOR_2048_CPS;  break;
        case 12:    sf = RFM95W_MODEM_CONFIG_2::SPREADING_FACTOR_4096_CPS;  break;
        default:
            if (sf < 6)         sf = RFM95W_MODEM_CONFIG_2::SPREADING_FACTOR_64_CPS;
            else if (sf > 12)   sf = RFM95W_MODEM_CONFIG_2::SPREADING_FACTOR_4096_CPS;
            break;
    }

    // Set spreading factor
    writeRegister(RFM95W_REG::MODEM_CONFIG_2, (readRegister(RFM95W_REG::MODEM_CONFIG_2) & ~RFM95W_MODEM_CONFIG_2::SPREADING_FACTOR | sf));

    // Check if low data rate bit should be set or cleared
    setLowDatarate();
}

/// @brief Set the signal bandwidth
/// @param sbw Signal bandwidth in Hz
void RFM95W::setSignalBandwidth(long sbw) {
    uint8_t bw;
    if      (sbw <= 7800)   bw = RFM95W_MODEM_CONFIG_1::BW_7_8KHZ;
    else if (sbw <= 10400)  bw = RFM95W_MODEM_CONFIG_1::BW_10_4KHZ;
    else if (sbw <= 15600)  bw = RFM95W_MODEM_CONFIG_1::BW_15_6KHZ;
    else if (sbw <= 20800)  bw = RFM95W_MODEM_CONFIG_1::BW_20_8KHZ;
    else if (sbw <= 31250)  bw = RFM95W_MODEM_CONFIG_1::BW_31_25KHZ;
    else if (sbw <= 41700)  bw = RFM95W_MODEM_CONFIG_1::BW_41_7KHZ;
    else if (sbw <= 62500)  bw = RFM95W_MODEM_CONFIG_1::BW_62_5KHZ;
    else if (sbw <= 125000) bw = RFM95W_MODEM_CONFIG_1::BW_125_KHZ;
    else if (sbw <= 250000) bw = RFM95W_MODEM_CONFIG_1::BW_250_KHZ;
    else                    bw = RFM95W_MODEM_CONFIG_1::BW_500_KHZ;

    // Set the modem bandwidth
    writeRegister(RFM95W_REG::MODEM_CONFIG_1, (readRegister(RFM95W_REG::MODEM_CONFIG_1) & ~RFM95W_MODEM_CONFIG_1::BW) | bw);

    // Check if the low data rate bit should be set or cleared
    setLowDatarate();
}

/// @brief Sets the data rate optimization if the bandwidth and spreading factor settings call for it.
void RFM95W::setLowDatarate() {
    uint8_t BW = readRegister(RFM95W_REG::MODEM_CONFIG_1) >> 4;
    uint8_t SF = readRegister(RFM95W_REG::MODEM_CONFIG_2) >> 4;

    float bw_tab[] = {7800, 10400, 15600, 20800, 31250, 41700, 62500, 125000, 250000, 500000};

    float bandwidth = bw_tab[BW];

    float symbolTime = 1000.0f * pow(2, SF) / bandwidth;

    uint8_t current = readRegister(RFM95W_REG::MODEM_CONFIG_3) & ~RFM95W_MODEM_CONFIG_3::LOW_DATA_RATE_OPTIMIZE;
    if (symbolTime > 16.0)  writeRegister(RFM95W_REG::MODEM_CONFIG_3, current | RFM95W_MODEM_CONFIG_3::LOW_DATA_RATE_OPTIMIZE);
    else                    writeRegister(RFM95W_REG::MODEM_CONFIG_3, current);
}

/// @brief Set the coding rate to 4/5, 4/6, 4/7 or 4/8
/// @param denominator Ranging from 4-8, both included
void RFM95W::setCodingRate4(uint8_t denominator) {
    int cr;
    if (denominator <= 5)       cr = RFM95W_MODEM_CONFIG_1::CODING_RATE_4_5;
    else if (denominator == 6)  cr = RFM95W_MODEM_CONFIG_1::CODING_RATE_4_6;
    else if (denominator == 7)  cr = RFM95W_MODEM_CONFIG_1::CODING_RATE_4_7;
    else if (denominator >= 8)  cr = RFM95W_MODEM_CONFIG_1::CODING_RATE_4_8;

    writeRegister(RFM95W_REG::MODEM_CONFIG_1, (readRegister(RFM95W_REG::MODEM_CONFIG_1) & ~RFM95W_MODEM_CONFIG_1::CODING_RATE) | cr);
}

/// @brief Check if the channel is currently active
/// @return True if channel is active, false if channel is not active
bool RFM95W::isChannelActive() {
    if (_mode != RFM95W_MODE::cad) {
        writeRegister(RFM95W_REG::OP_MODE, RFM95W_OP_MODE::CAD);
        writeRegister(RFM95W_REG::DIO_MAPPING_1, 0x80); // Interrrupt on CAD done
        _mode = RFM95W_MODE::cad;
    }

    while (_mode == RFM95W_MODE::cad)
        YIELD; // Need to replace this with something else

    return _cad;
}

/// @brief Get the last measured SNR
/// @return The SNR in dB
int RFM95W::lastSNR() {
    return _lastSNR;
}

/// @brief Get the maximum allowable message length
/// @return Message length in bytes
uint8_t RFM95W::maxMessageLength() {
    return (uint8_t)251;
}

/// @brief Get the frequency error
/// @return Estimated center frequency offset in Hz of last received message
int RFM95W::frequencyError() {
    int32_t freqerror = 0;
    freqerror = readRegister(RFM95W_REG::FEI_MSB);
    freqerror <<= 8;
    freqerror |= readRegister(RFM95W_REG::FEI_MID);
    freqerror <<= 8;
    freqerror |= readRegister(RFM95W_REG::FEI_LSB);

    if (freqerror & 0x80000) freqerror |= 0xfff00000;

    int error = 0;
    float bw_tab[] = {7.8f, 10.4f, 15.6f, 20.8f, 31.25f, 31.7f, 62.5f, 125.0f, 250.0f, 500.0f};
    uint8_t bwindex = readRegister(RFM95W_REG::MODEM_CONFIG_1) >> 4;
    if (bwindex < (sizeof(bw_tab) / sizeof(float))) error = (float)freqerror * bw_tab[bwindex] * ((float)(1L << 24) / (float)RFM95W_FXOSC / 5000f);
    return error;
}