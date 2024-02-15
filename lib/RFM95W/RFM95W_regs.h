/* 
 * Title:       RFM95W Library
 * Description: A software library for the RFM95W module based on the RadioHead library
 * Author:      
 */

#pragma once

namespace RFM95W_REG {
    const uint8_t FIFO                      = (0x00);
    const uint8_t OP_MODE                   = (0x01);
    const uint8_t FRF_MSB                   = (0x06);
    const uint8_t FRF_MID                   = (0x07);
    const uint8_t FRF_LSB                   = (0x08);
    const uint8_t PA_CONFIG                 = (0x09);
    const uint8_t PA_RAMP                   = (0x0a);
    const uint8_t OCP                       = (0x0b);
    const uint8_t LNA                       = (0x0c);
    const uint8_t FIFO_ADDR_PTR             = (0x0d);
    const uint8_t FIFO_TX_BASE_ADDR         = (0x0e);
    const uint8_t FIFO_RX_BASE_ADDR         = (0x0f);
    const uint8_t FIFO_RX_CURRENT_ADDR      = (0x10);
    const uint8_t IRQ_FLAGS_MASK            = (0x11);
    const uint8_t IRQ_FLAGS                 = (0x12);
    const uint8_t RX_NB_BYTES               = (0x13);
    const uint8_t RX_HEADER_CNT_VALUE_MSB   = (0x14);
    const uint8_t RX_HEADER_CNT_VALUE_LSB   = (0x15);
    const uint8_t RX_PACKET_CNT_VALUE_MSB   = (0x16);
    const uint8_t RX_PACKET_CNT_VALUE_MSB   = (0x17);
    const uint8_t MODEM_STAT                = (0x18);
    const uint8_t PKT_SNR_VALUE             = (0x19);
    const uint8_t PKT_RSSI_VALUE            = (0x1a);
    const uint8_t RSSI_VALUE                = (0x1b);
    const uint8_t HOP_CHANNEL               = (0x1c);
    const uint8_t MODEM_CONFIG_1            = (0x1d);
    const uint8_t MODEM_CONFIG_2            = (0x1e);
    const uint8_t SYMB_TIMEOUT_LSB          = (0x1f);
    const uint8_t PREAMBLE_MSB              = (0x20);
    const uint8_t PREAMBLE_LSB              = (0x21);
    const uint8_t PAYLOAD_LENGTH            = (0x22);
    const uint8_t MAX_PAYLOAD_LENGTH        = (0x23);
    const uint8_t HOP_PERIOD                = (0x24);
    const uint8_t FIFO_RX_BYTE_ADDR         = (0x25);
    const uint8_t MODEM_CONFIG_3            = (0x26);

    const uint8_t DIO_MAPPING_1             = (0x40);
    const uint8_t DIO_MAPPING_2             = (0x41);
    const uint8_t VERSION                   = (0x42);

    const uint8_t TCXO                      = (0x4b);
    const uint8_t PA_DAC                    = (0x4d);
    const uint8_t FORMER_TEMP               = (0x5b);
    const uint8_t AGC_REF                   = (0x61);
    const uint8_t AGC_THRESH_1              = (0x62);
    const uint8_t AGC_THRESH_2              = (0x63);
    const uint8_t AGC_THRESH_3              = (0x64);
};

namespace RFM95W_OP_MODE {
    const uint8_t LONG_RANGE_MODE   = (0x80);
    const uint8_t ACCESS_SHARED_REG = (0x40);
    const uint8_t MODE              = (0x07); /* CHECK THIS ONE OUT? */
    const uint8_t SLEEP             = (0x00);
    const uint8_t STDBY             = (0x01);
    const uint8_t FSTX              = (0x02);
    const uint8_t TX                = (0x03);
    const uint8_t FSRX              = (0x04);
    const uint8_t RXCONTINUOUS      = (0x05);
    const uint8_t RXSINGLE          = (0x06);
    const uint8_t CAD               = (0x07);
};

namespace RFM95W_PA_CONFIG {
    const uint8_t PA_SELECT     = (0x80);
    const uint8_t MAX_POWER     = (0x70);
    const uint8_t OUTPUT_POWER  = (0x0f);
};

namespace RFM95W_PA_RAMP {
    const uint8_t LOW_PN_TX_PLL_OFF     = (0x10);
    const uint8_t PA_RAMP               = (0x0f);
    const uint8_t PA_RAMP_3_4_MS        = (0x00);
    const uint8_t PA_RAMP_2_MS          = (0x01);
    const uint8_t PA_RAMP_1_MS          = (0x02);
    const uint8_t PA_RAMP_500_US        = (0x03);
    const uint8_t PA_RAMP_250_US        = (0x04); /* Check datasheet */
    const uint8_t PA_RAMP_125_US        = (0x05);
    const uint8_t PA_RAMP_100_US        = (0x06);
    const uint8_t PA_RAMP_62_US         = (0x07);
    const uint8_t PA_RAMP_50_US         = (0x08);
    const uint8_t PA_RAMP_40_US         = (0x09);
    const uint8_t PA_RAMP_31_US         = (0x0a);
    const uint8_t PA_RAMP_25_US         = (0x0b);
    const uint8_t PA_RAMP_20_US         = (0x0c);
    const uint8_t PA_RAMP_15_US         = (0x0d);
    const uint8_t PA_RAMP_12_US         = (0x0e);
    const uint8_t PA_RAMP_10_US         = (0x0f);
};

namespace RFM95W_OCP {
    const uint8_t ON    = (0x20);
    const uint8_t TRIM  = (0x1f);
};

namespace RFM95W_LNA {
    const uint8_t GAIN          = (0xe0);
    const uint8_t BOOST         = (0x03);
    const uint8_t BOOST_DEFAULT = (0x00);
    const uint8_t BOOST_150_PC  = (0x11);
};

namespace RFM95W_IRQ_FLAGS_MASK {
    const uint8_t RX_TIMEOUT            = (0x80);
    const uint8_t RX_DONE               = (0x40);
    const uint8_t PAYLOAD_CRC_ERROR     = (0x20);
    const uint8_t VALID_HEADER          = (0x10);
    const uint8_t TX_DONE               = (0x08);
    const uint8_t CAD_DONE              = (0x04);
    const uint8_t FHSS_CHANGE_CHANNEL   = (0x02);
    const uint8_t CAD_DETECTED          = (0x01);
};

namespace RFM95W_IRQ_FLAGS {
    const uint8_t RX_TIMEOUT            = (0x80);
    const uint8_t RX_DONE               = (0x40);
    const uint8_t PAYLOAD_CRC_ERROR     = (0x20);
    const uint8_t VALID_HEADER          = (0x10);
    const uint8_t TX_DONE               = (0x08);
    const uint8_t CAD_DONE              = (0x04);
    const uint8_t FHSS_CHANGE_CHANNEL   = (0x02);
    const uint8_t CAD_DETECTED          = (0x01);
};

namespace RFM95W_MODEM_STAT {
    const uint8_t RX_CODING_RATE        = (0xe0);
    const uint8_t CLEAR                 = (0x10);
    const uint8_t HEADER_INF_VALID      = (0x08);
    const uint8_t RX_ONGOING            = (0x04);
    const uint8_t SIGNAL_SYNCHRONIZED   = (0x02);
    const uint8_t SIGNAL_DETECTED       = (0x01);
};

namespace RFM95W_HOP_CHANNEL {
    const uint8_t PLL_TIMEOUT           = (0x80);
    const uint8_t RX_PAYLOAD_CRC_IS_ON  = (0x40);
    const uint8_t FHSS_PRESENT_CHANNEL  = (0x3f);
};

namespace RFM95W_MODEM_CONFIG_1 {
    const uint8_t BW                        = (0xc0);
    const uint8_t BW_125_KHZ                = (0x00);
    const uint8_t BW_250_KHZ                = (0x40);
    const uint8_t BW_500_KHZ                = (0x80);

    const uint8_t CODING_RATE               = (0x38);
    const uint8_t CODING_RATE_4_5           = (0x00);
    const uint8_t CODING_RATE_4_6           = (0x08);
    const uint8_t CODING_RATE_4_7           = (0x10);
    const uint8_t CODING_RATE_4_8           = (0x18);

    const uint8_t IMPLICIT_HEADER_MODE_ON   = (0x04);
    const uint8_t RX_PAYLOAD_CRC_ON         = (0x02),
    const uint8_t LOW_DATA_RATE_OPTIMIZE    = (0x01);
};

namespace RM95W_MODEM_CONFIG_2 {
    const uint8_t SPREADING_FACTOR          = (0xf0);
    const uint8_t SPREADING_FACTOR_64_CPS   = (0x60);
    const uint8_t SPREADING_FACTOR_128_CPS  = (0x70);
    const uint8_t SPREADING_FACTOR_256_CPS  = (0x80);
    const uint8_t SPREADING_FACTOR_512_CPS  = (0x90);
    const uint8_t SPREADING_FACTOR_1024_CPS = (0xa0);
    const uint8_t SPREADING_FACTOR_2048_CPS = (0xb0);
    const uint8_t SPREADING_FACTOR_4096_CPS = (0xc0);

    const uint8_t RX_CONTINUOUS_MODE        = (0x08);
    const uint8_t AGC_AUTO_ON               = (0x04);
    const uint8_t SYM_TIMEOUT_MSB           = (0x03);
};

namespace RFM95W_PA_DAC {
    const uint8_t DISABLE                   = (0x04);
    const uint8_t ENABLE                    = (0x07);
};