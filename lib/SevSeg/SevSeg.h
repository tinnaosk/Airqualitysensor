#pragma once
#include <hardware/i2c.h>

const uint8_t SevSeg_DEFAULT_I2CADDR = 0x70;
const uint8_t SevSeg_BLINK_CMD = 0x80; // I2C reg for BLINK setting on HT16K33
const uint8_t SevSeg_BLINK_DISPLAY_ON = 0x01; // I2C for steady on

const uint8_t LED_ON = 1;
const uint8_t LED_OFF = 0;

const uint8_t SevSeg_BRIGHTNESS_CMD = 0xE0; // I2C reg for the brightness settings
const uint8_t SevSeg_Digits = 5; // no. digits in 7 seg display plys nul end

/*
Segment names for 14-segment alphanumeric displays.
    -------A-------
    |\     |     /|
    | \    J    / |
    |   H  |  K   |
    F    \ | /    B
    |     \|/     |
    |--G1--|--G2--|
    |     /|\     |
    E    / | \    C
    |   L  |   N  |
    | /    M    \ |
    |/     |     \|
    -------D-------  DP
*/

const uint16_t SevSeg_A  = 0b0000000000000001;
const uint16_t SevSeg_B  = 0b0000000000000010;
const uint16_t SevSeg_C  = 0b0000000000000100;
const uint16_t SevSeg_D  = 0b0000000000001000;
const uint16_t SevSeg_E  = 0b0000000000010000;
const uint16_t SevSeg_F  = 0b0000000000100000;
const uint16_t SevSeg_G1 = 0b0000000001000000;
const uint16_t SevSeg_G2 = 0b0000000010000000;
const uint16_t SevSeg_H  = 0b0000000100000000;
const uint16_t SevSeg_J  = 0b0000001000000000;
const uint16_t SevSeg_K  = 0b0000010000000000;
const uint16_t SevSeg_L  = 0b0000100000000000;
const uint16_t SevSeg_M  = 0b0001000000000000;
const uint16_t SevSeg_N  = 0b0010000000000000;
const uint16_t SevSeg_DP = 0b0100000000000000;

#define RAW_BITS 1
#define DEC 2


enum class BLINK_RATE : uint8_t {
    Blink_off = 0,
    Blink_2hz,
    Blink_1hz,
    Blink_halfhz, // 0.5 Hz blink
};

class SevSeg {
public:
    SevSeg() {};
    SevSeg(i2c_inst_t *i2c, uint8_t addr = SevSeg_DEFAULT_I2CADDR) : _i2c(i2c), SevSeg_ADDRESS(addr) {};

    void setDisplayState(bool state);
    void setBrightness(uint8_t b);
    void blinkRate(BLINK_RATE b);
    void writeDisplay(void);
    void clear(void);
    void begin(void);
    uint16_t displaybuffer[8]; // Raw display data

    size_t write(char c);
    size_t write(const char *buffer, size_t size);
    
    void print(char c);
    void print(unsigned char b, int base);
    void print(int n, int base);
    void print(unsigned int n, int base);
    void print(long n, int base);
    void print(unsigned long n, int base);
    void print(double n, int digits);
    void print(const char c[]);

    void println(char c);
    void println(unsigned char b, int base);
    void println(int n, int base);
    void println(unsigned int n, int base);
    void println(long n, int base);
    void println(unsigned long n, int base);
    void println(double n, int digits);
    void println(const char c[]);
    void println(void);

    void writeDigitRaw(uint8_t x, uint8_t bitmask);
    void writeDigitNum(uint8_t x, uint8_t num, bool dot);
    void writeDigitAscii(uint8_t x, uint8_t c, bool dot);

    void drawColon(bool state);

    void printNumber(long n, uint8_t base);
    void printFloat(double n, uint8_t fracDigits, uint8_t base);
    void printError(void);
    void writeColon(void);

private:
    uint8_t position;
    i2c_inst_t* _i2c;
    uint8_t SevSeg_ADDRESS;
    
};