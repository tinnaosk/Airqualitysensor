/* 
 * Title:       RFM95W Library
 * Description: A software library for the RFM95W module based on the RadioHead library
 * Author:      
 */

#pragma once

#include "RFM95W_regs.h"
#include <hardware/spi.h>

/* TODO:
 *  - Change setModemConfig to use better naming
 *  - Change ModemConfigChoice to use better naming
 *  - Find a better way to do the define stuff
*/

#define RFM95W_FXOSC 32000000.0f
#define RFM95W_FSTEP (RFM95W_FXOSC / 524288)

class RFM95W {
public:
    RFM95W() {};
    RFM95W(hardware_spi_t* spi, uint8_t cs_pin) : _spi(spi), _cs_pin(cs_pin) {};

    struct ModemConfig {
        uint8_t reg_1d;
        uint8_t reg_1e;
        uint8_t reg_26;
    };

    enum class ModemConfigChoice : uint8_t {
        Bw125Cr45Sf128 = 0,
        Bw500Cr45Sf128,
        Bw31_25Cr48Sf512,
        Bw125Cr48Sf4096,
        Bw125Cr45Sf2048
    };

    void init();

    void setMode(RFM95W_MODE mode);
    void setTxPower(int8_t power, bool useRFO=false);
    void setPreambleLength(uint16_t bytes);
    bool setFrequency(float center);
    void enableTCXO(bool enabled=true);
    void setSpreadingFactor(uint8_t sf);
    void setSignalBandwidth(long sbw);
    void setLowDatarate();
    void setCodingRate4(uint8_t denominator);
    int lastSNR();
    uint8_t maxMessageLength();
    int frequencyError();
    
    bool isChannelActive(); // Needs to be checked out, what is YIELD and _cad? We don't have an OS!

    // From RH_RF95 class
    bool printRegisters();
    void setModemRegister(const ModemConfig* config);
    bool setModemConfig(ModemConfigChoice index);
    bool available();
    bool recv(uint8_t* buf, uint8_t* len);
    bool send(const uint8_t* data, uint8_t len);
    void setPayloadCRC(bool enabled);
    uint8_t getDeviceVersion();

private:
    spi_inst_t* _spi;
    uint8_t _cs_pin;
    RFM95W_MODE _mode;
    bool _usingHFport = false;
    int8_t _lastSNR;

    bool writeRegister(uint8_t address, uint8_t data);
    uint8_t readRegister(uint8_t address);

    // From RH_RF95 class
    bool setupInterruptHandler();
    void handleInterrupt();
    void validateRxBuf();
    void clearRxBuf();
    bool modeWillChange(RHMode);
};