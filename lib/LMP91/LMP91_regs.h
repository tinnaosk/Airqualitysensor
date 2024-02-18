#pragma once


const uint8_t LMP91_DEFAULT_I2CADDR = 0x90;

const uint8_t LMP91_STATUS = 0x00;  // Power-on status 
const uint8_t LMP91_LOCK = 0x01;    // Enables and disables writing of the TIACN and REFCN
const uint8_t LMP91_TIACN = 0x10;   // Transimpedance gaind and the load resistance
const uint8_t LMP91_REFCN = 0x11;   // Reference control register
const uint8_t LMP91_MODE = 0x12;    // Mode register, configures the operation modes

/// @brief Status of device
enum class STATUS : uint8_t {
    Status_not_ready = 0, // Default
    Status_ready,
};

/// @brief Write protection
enum class LOCK : uint8_t {
    Lock_reg_write_mode = 0,
    Lock_reg_read_mode, // Default
};

/// @brief TIA feedback resistance selection
enum class TIA_GAIN : uint8_t {
    Gain_External = 0, // Default
    Gain_2k75,
    Gain_3k5,
    Gain_7k,
    Gain_14k,
    Gain_35k,
    Gain_120k,
    Gain_350k,
};

/// @brief R_Load selection
enum class R_LOAD : uint8_t {
    Load_10 = 0,
    Load_33,
    Load_50,
    Load_100, // Default
};

/// @brief Reference voltage source selection
enum class REF_SOURCE : uint8_t {
    Source_internal = 0, // Default
    Source_external,
};

/// @brief Internal zero selection (Percentage of the source reference)
enum class INT_Z : uint8_t {
    Int_20 = 0,
    Int_50, // Default
    Int_67,
    Int_Internal, // Internal zero circuitry bypassed
};

/// @brief Selection of Bias polariyt
enum class BIAS_SIGN : uint8_t {
    Sign_Neg = 0, // Default
    Sign_Pos,
};

/// @brief BIAS selection (Percentage of the source reference)
enum class BIAS : uint8_t {
    Bias_1 = 0, // Default
    Bias_2,
    Bias_4,
    Bias_6,
    Bias_8,
    Bias_10,
    Bias_12,
    Bias_14,
    Bias_16,
    Bias_18,
    Bias_20,
    Bias_22,
    Bias_24,
};

/// @brief shorting FET feature
enum class FET_SHORT : uint8_t {
    Short_Disabled = 0, // Default
    Short_Enabled,
};

/// @brief Mode of operation selection
enum class OP_MODE : uint8_t {
    Mode_Deep_Sleep = 0, // Default
    Mode_2_Lead_Ground,
    Mode_Standby,
    Mode_3_Lead_Amperpmetric,
    Mode_Temp_TIA_Off,
    Mode_Temp_TIA_On,
};