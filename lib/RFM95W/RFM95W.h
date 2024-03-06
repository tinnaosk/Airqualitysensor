/* 
 * Title:       RFM95W Library
 * Description: A software library for the RFM95W module based on the RadioHead library
 * Author:      
 */

#pragma once

class RFM95W {
public:
    RFM95W();

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

    bool init();

    // From RH_RF95 class
    bool printRegisters();
    void setModemRegister(const ModemConfig* config);
    bool setModemConfig(ModemConfigChoice index);
    virtual bool available();
    virtual bool recv(uint8_t* buf, uint8_t* len);
    virtual bool send(const uint8_t* data, uint8_t len);
    void setPreambleLength(uint16_t bytes);
    virtual uint8_t maxMessageLength();
    bool setFrequency(float centre);
    void setModeIdle();
    void setModeRx();
    void setModeTx();
    void setTxPower(int8_t power, bool useRFO=false);
    virtual bool sleep();
    virtual bool isChannelActive();
    void enableTCXO(bool enabled=true);
    int frequencyError();
    int lastSNR();
    void setSpreadingFactor(uint8_t sf);
    void setSignalBandwidth(long sbw);
    void setCodingRate4(uint8_t denominator);
    void setLowDatarate();
    void setPayloadCRC(bool enabled);
    uint8_t getDeviceVersion();

private:
    bool writeRegister(uint8_t address, uint8_t data);
    bool readRegister(uint8_t address, uint8_t* data);

    // From RH_RF95 class
    bool setupInterruptHandler();
    void handleInterrupt();
    void validateRxBuf();
    void clearRxBuf();
    virtual bool modeWillChange(RHMode);
};