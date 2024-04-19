#include <string.h>
#include <stdio.h>
#include <hardware/i2c.h>
#include "SevSeg.h"

static const uint8_t sevensegfonttable[] = {
    0b00000000, // (space)
    0b10000110, // !
    0b00100010, // "
    0b01111110, // #
    0b01101101, // $
    0b11010010, // %
    0b01000110, // &
    0b00100000, // '
    0b00101001, // (
    0b00001011, // )
    0b00100001, // *
    0b01110000, // +
    0b00010000, // ,
    0b01000000, // -
    0b10000000, // .
    0b01010010, // /
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111, // 9
    0b00001001, // :
    0b00001101, // ;
    0b01100001, // <
    0b01001000, // =
    0b01000011, // >
    0b11010011, // ?
    0b01011111, // @
    0b01110111, // A
    0b01111100, // B
    0b00111001, // C
    0b01011110, // D
    0b01111001, // E
    0b01110001, // F
    0b00111101, // G
    0b01110110, // H
    0b00110000, // I
    0b00011110, // J
    0b01110101, // K
    0b00111000, // L
    0b00010101, // M
    0b00110111, // N
    0b00111111, // O
    0b01110011, // P
    0b01101011, // Q
    0b00110011, // R
    0b01101101, // S
    0b01111000, // T
    0b00111110, // U
    0b00111110, // V
    0b00101010, // W
    0b01110110, // X
    0b01101110, // Y
    0b01011011, // Z
    0b00111001, // [
    0b01100100, //
    0b00001111, // ]
    0b00100011, // ^
    0b00001000, // _
    0b00000010, // `
    0b01011111, // a
    0b01111100, // b
    0b01011000, // c
    0b01011110, // d
    0b01111011, // e
    0b01110001, // f
    0b01101111, // g
    0b01110100, // h
    0b00010000, // i
    0b00001100, // j
    0b01110101, // k
    0b00110000, // l
    0b00010100, // m
    0b01010100, // n
    0b01011100, // o
    0b01110011, // p
    0b01100111, // q
    0b01010000, // r
    0b01101101, // s
    0b01111000, // t
    0b00011100, // u
    0b00011100, // v
    0b00010100, // w
    0b01110110, // x
    0b01101110, // y
    0b01011011, // z
    0b01000110, // {
    0b00110000, // |
    0b01110000, // }
    0b00000001, // ~
    0b00000000, // del
};

static const uint16_t alphafonttable[] = {

    0b0000000000000001, 0b0000000000000010, 0b0000000000000100,
    0b0000000000001000, 0b0000000000010000, 0b0000000000100000,
    0b0000000001000000, 0b0000000010000000, 0b0000000100000000,
    0b0000001000000000, 0b0000010000000000, 0b0000100000000000,
    0b0001000000000000, 0b0010000000000000, 0b0100000000000000,
    0b1000000000000000, 0b0000000000000000, 0b0000000000000000,
    0b0000000000000000, 0b0000000000000000, 0b0000000000000000,
    0b0000000000000000, 0b0000000000000000, 0b0000000000000000,
    0b0001001011001001, 0b0001010111000000, 0b0001001011111001,
    0b0000000011100011, 0b0000010100110000, 0b0001001011001000,
    0b0011101000000000, 0b0001011100000000,
    0b0000000000000000, //
    0b0000000000000110, // !
    0b0000001000100000, // "
    0b0001001011001110, // #
    0b0001001011101101, // $
    0b0000110000100100, // %
    0b0010001101011101, // &
    0b0000010000000000, // '
    0b0010010000000000, // (
    0b0000100100000000, // )
    0b0011111111000000, // *
    0b0001001011000000, // +
    0b0000100000000000, // ,
    0b0000000011000000, // -
    0b0100000000000000, // .
    0b0000110000000000, // /
    0b0000110000111111, // 0
    0b0000000000000110, // 1
    0b0000000011011011, // 2
    0b0000000010001111, // 3
    0b0000000011100110, // 4
    0b0010000001101001, // 5
    0b0000000011111101, // 6
    0b0000000000000111, // 7
    0b0000000011111111, // 8
    0b0000000011101111, // 9
    0b0001001000000000, // :
    0b0000101000000000, // ;
    0b0010010000000000, // <
    0b0000000011001000, // =
    0b0000100100000000, // >
    0b0001000010000011, // ?
    0b0000001010111011, // @
    0b0000000011110111, // A
    0b0001001010001111, // B
    0b0000000000111001, // C
    0b0001001000001111, // D
    0b0000000011111001, // E
    0b0000000001110001, // F
    0b0000000010111101, // G
    0b0000000011110110, // H
    0b0001001000001001, // I
    0b0000000000011110, // J
    0b0010010001110000, // K
    0b0000000000111000, // L
    0b0000010100110110, // M
    0b0010000100110110, // N
    0b0000000000111111, // O
    0b0000000011110011, // P
    0b0010000000111111, // Q
    0b0010000011110011, // R
    0b0000000011101101, // S
    0b0001001000000001, // T
    0b0000000000111110, // U
    0b0000110000110000, // V
    0b0010100000110110, // W
    0b0010110100000000, // X
    0b0001010100000000, // Y
    0b0000110000001001, // Z
    0b0000000000111001, // [
    0b0010000100000000, //
    0b0000000000001111, // ]
    0b0000110000000011, // ^
    0b0000000000001000, // _
    0b0000000100000000, // `
    0b0001000001011000, // a
    0b0010000001111000, // b
    0b0000000011011000, // c
    0b0000100010001110, // d
    0b0000100001011000, // e
    0b0000000001110001, // f
    0b0000010010001110, // g
    0b0001000001110000, // h
    0b0001000000000000, // i
    0b0000000000001110, // j
    0b0011011000000000, // k
    0b0000000000110000, // l
    0b0001000011010100, // m
    0b0001000001010000, // n
    0b0000000011011100, // o
    0b0000000101110000, // p
    0b0000010010000110, // q
    0b0000000001010000, // r
    0b0010000010001000, // s
    0b0000000001111000, // t
    0b0000000000011100, // u
    0b0010000000000100, // v
    0b0010100000010100, // w
    0b0010100011000000, // x
    0b0010000000001100, // y
    0b0000100001001000, // z
    0b0000100101001001, // {
    0b0001001000000000, // |
    0b0010010010001001, // }
    0b0000010100100000, // ~
    0b0011111111111111,
};

/// @brief Turn display on or off
/// @param state State: true = on, false = off
void SevSeg::setDisplayState(bool state) {
    uint8_t buffer[1]{0};
    if(state)   buffer[0] = SevSeg_BLINK_CMD | 1;
    else        buffer[0] = SevSeg_BLINK_CMD;
    i2c_write_timeout_us(_i2c, SevSeg_ADDRESS, buffer, 1, false, 10000);
}

/// @brief Set the brightness of the display
/// @param b Brightness: 0 (min), 15 (max)
void SevSeg::setBrightness(uint8_t b) {
    if (b > 15) b = 15;
    uint8_t buffer[1]{0};
    buffer[0] = SevSeg_BRIGHTNESS_CMD | b;
    i2c_write_timeout_us(_i2c, SevSeg_ADDRESS, buffer, 1, false, 10000);
}

/// @brief Set display blink rate
/// @param blink set as either, off, 2 Hz, 1 Hz or 0.5 Hz rate.
void SevSeg::blinkRate(BLINK_RATE b){
    uint8_t rate = (uint8_t)b;
    if ((uint8_t)b > 3) rate = 0;
    uint8_t buffer[1]{0};
    buffer[0] = SevSeg_BLINK_CMD | SevSeg_BLINK_DISPLAY_ON | (rate << 1);
    i2c_write_timeout_us(_i2c, SevSeg_ADDRESS, buffer, 1, false, 10000);
}

/// @brief Issue buffered data in RAM to display
void SevSeg::writeDisplay(void) {
    uint8_t buffer[17]{0};
    buffer[0] = 0x00;

    for(uint8_t i = 0; i < 8; i++) {
        buffer[1+2*i] = displaybuffer[i] & 0xFF;
        buffer[2+2*i] = displaybuffer[i] >> 8;
    }
    i2c_write_timeout_us(_i2c, SevSeg_ADDRESS, buffer, 17, false, 10000);
}

/// @brief Clear display
/// @param  
void SevSeg::clear(void){
    for(uint8_t i = 0; i < 8; i++) {
        displaybuffer[i] = 0;
    }
}

/// @brief Start I2C and initialize display state (blink off, full brightness).
/// @param 
void SevSeg::begin(void) {
    uint8_t buffer[1] = {0x21};
    i2c_write_timeout_us(_i2c, SevSeg_ADDRESS, buffer, 1, false, 10000);

    clear();
    blinkRate(BLINK_RATE::Blink_off);
    setBrightness(15);
}

/// @brief Issue single character to display
/// @param c Character to write (ASCII character, not numeric)
/// @return 1 if character is written, else 0
size_t SevSeg::write(char c) {
    uint8_t r = 0;
    if (c == '\n') position = 0;
    if (c == '\r') position = 0;

    if ((c >= ' ') && (c <= 127)) {
        writeDigitAscii(position, c, false);
        r = 1;
    }
    position++;
    if (position == 2)  position++;

    return r;
}

/// @brief  Write character from buffer to display
/// @param  buffer Charcter array to write
/// @param  size Number of characters to write
/// @return Number of characters written    
size_t SevSeg::write(const char *buffer, size_t size) {
    size_t n = 0;

    while (n < size) {
        write(buffer[n]);
        n++;
    }
    
    for (uint8_t i = position; i < 5; i++) {
        writeDigitRaw(i, 0x00);
    }

    return n;
}

/// @brief Print byte-size numeric value to 7-segment display
/// @param c Numeric value
void SevSeg::print(char c) {
    write(c);
}

/// @brief Print unsigned byte-size numeric value to 7-segment display
/// @param b    Numeric value 
/// @param base Number base (default = RAW_BITS = raw bits)
void SevSeg::print(unsigned char b, int base = RAW_BITS) {
    print((unsigned long)b, base);
}

/// @brief Print integer value to 7-segment display
/// @param n    Numeric value
/// @param base Number base (default = DEC = base 10)
void SevSeg::print(int n, int base = DEC) { 
    print((long)n, base); 
    }

/// @brief Print unsigned integer value to 7-segment display
/// @param n    Numeric value
/// @param base Number base (default = DEC = base 10)
void SevSeg::print(unsigned int n, int base = DEC) {
    print((unsigned long)n, base);
}

/// @brief Print long integer value to 7-segment display
/// @param n    Numeric value
/// @param base Number base (default = DEC = base 10)
void SevSeg::print(long n, int base = DEC) {
    printNumber(n, base);
}

/// @brief Print unsinged long integer value to 7-segment display
/// @param n    Numeric value
/// @param base Number base (default = DEC = base 10)
void SevSeg::print(unsigned long n, int base = DEC) {
    if (base == 0)  write(n);
    else            printNumber(n, base);
}

/// @brief Print double-precision float value to 7-segment display
/// @param n    Numeric value
/// @param digits Fractional-part digits
void SevSeg::print(double n, int digits = 2) {
    printFloat(n, digits, DEC);
}

/// @brief Print from a C-style string array to 7-segment display
/// @param c Numeric value
void SevSeg::print(const char c[]) { 
    write(c, strlen(c)); 
    }

/// @brief Print byte-size numeric value w/newline to 7-segment
/// @param c Numeric value
void SevSeg::println(char c) {
    print(c);
    println();
}

/// @brief Print unsigned byte-size numeric value w/newline to 7-segment display
/// @param b    Numeric value
/// @param base Number base (Default = RAW_BITS = raw bits)
void SevSeg::println(unsigned char b, int base = RAW_BITS) {
    print(b, base);
    println();
}

/// @brief Print integer value w/newline to 7-segment display
/// @param n    Numeric value
/// @param base Number base (Default = DEC = base 10)
void SevSeg::println(int n, int base = DEC) {
    print(n, base);
    println();
}

/// @brief Print unsigned integer value w/newline to 7-segment display
/// @param n    Numeric value
/// @param base Number base (Default = DEC = base 10)
void SevSeg::println(unsigned int n, int base = DEC) {
    print(n, base);
    println();
}

/// @brief Print long integer value w/newline to 7-segment display
/// @param n    Numeric value
/// @param base Number base (Default = DEC = base 10)
void SevSeg::println(long n, int base = DEC) {
    print(n, base);
    println();
}

/// @brief Print unsigned lon integer value w/newline to 7-segment display
/// @param n    Numeric value
/// @param base Number base (Default = DEC = base 10)
void SevSeg::println(unsigned long n, int base = DEC) {
    print(n, base);
    println();
}

/// @brief Print double-precision float value w/newline to 7-segment display
/// @param n    Numeric value
/// @param digits Fractional-part digits
void SevSeg::println(double n, int digits = 2) {
    print(n, digits);
    println();
}

/// @brief Print from a C-style string array w/newline to 7-segment display
/// @param c Array of characters
void SevSeg::println(const char c[]) {
    print(c);
    println();
}

/// @brief Print new line to 7-segment display (rewind position to start)
/// @param  
void SevSeg::println(void) {
    position = 0;
}

/// @brief Write raw segment bits into display buffer.
/// @param x    Character position (0-3)
/// @param bitmask Segment bits
void SevSeg::writeDigitRaw(uint8_t x, uint8_t bitmask) {
    if (x > 4)  return;
    displaybuffer[x] = bitmask;
}

/// @brief Set specific digit # to a character value
/// @param x    Character position
/// @param num  Character ASCII
/// @param dot  If true, light corresponding decimal
void SevSeg::writeDigitNum(uint8_t x, uint8_t num, bool dot = false) {
    if (x > 4 || num > 15)  return;
    if (num >= 10) {
        switch (num)
        {
        case 10:
            writeDigitAscii(x, 'a', dot);
            break;
        case 11:
            writeDigitAscii(x, 'B', dot);
            break;
        case 12:
            writeDigitAscii(x, 'C', dot);
            break;
        case 13:
            writeDigitAscii(x, 'd', dot);
            break;
        case 14:
            writeDigitAscii(x, 'E', dot);
            break;
        case 15:
            writeDigitAscii(x,'F', dot);
            break;
        }
    }

    else {
        writeDigitAscii(x, num + 48, dot);
    }
}

/// @brief Set or unset colon segment
/// @param x    Character position
/// @param c    Character ASCII
/// @param dot  If true, light corresponding decimal
void SevSeg::writeDigitAscii(uint8_t x, uint8_t c, bool dot = false) {
    if (x > 4)  return;

    uint8_t font = sevensegfonttable[c - 32];
    writeDigitRaw(x, font | (dot << 7));
}

/// @brief Set or unset colon segment
/// @param state 'true' to enable colon, 'false' for off
void SevSeg::drawColon(bool state) {
    if (state)  displaybuffer[2] = 0x2;
    else        displaybuffer[2] = 0;
}

/// @brief General integer-printing function used by some of the print() variants
/// @param n    Numberic value
/// @param base Base (2 = binary)
void SevSeg::printNumber(long n, uint8_t base = 2) {
    printFloat(n, 0, base);
}

/// @brief General float-printing function used by some of the print() variants.
/// @param n    Numeric value
/// @param fracDigits Fractional-part digits
/// @param base Base (Default = DEC = base 10)
void SevSeg::printFloat(double n, uint8_t fracDigits = 2, uint8_t base = DEC) {
    uint8_t numericDigits = 4;
    bool isNegative = false;

    if ( n < 0) {
        isNegative = true;
        --numericDigits;
        n *= -1;
    }

    double toIntFactor = 1.0;
    for (int i = 0; i < fracDigits; i++) {
        toIntFactor *= base;
    }

    uint32_t displayNumber = n * toIntFactor + 0.5;
    uint32_t tooBig = 1;
    for (int i = 0; i < numericDigits; i++) {
        tooBig *= base;
    }

    while (displayNumber >= tooBig) {
        --fracDigits;
        toIntFactor /= base;
        displayNumber = n * toIntFactor + 0.5;
    }

    if (toIntFactor < 1 ) {
        printError();
    } else {
        int8_t displayPos = 4;

        for (uint8_t i = 0; displayNumber || i <= fracDigits; i++) {
            bool displayDecimal = (fracDigits != 0 && i == fracDigits);
            writeDigitNum(displayPos--, displayNumber % base, displayDecimal);
            if (displayPos == 2) {
                writeDigitRaw(displayPos--, 0x00);
            }
            displayNumber /= base;
        }
        if (isNegative) writeDigitRaw(displayPos--, 0x40);

        while (displayPos >= 0) writeDigitRaw(displayPos--, 0x00); 
    }
}

/// @brief Light display degments in an error-indicationg configuration.
void SevSeg::printError(void) {
    for(uint8_t i = 0; i < SevSeg_Digits; i++) {
        writeDigitRaw(i, (i == 2 ? 0x00 : 0x40));
    }
}

/// @brief Issue colon-on directly to display (bypass buffer)
void SevSeg::writeColon(void) {
    uint8_t buffer[3];

    buffer[0] = 0x04;
    buffer[1] = displaybuffer[2] & 0xFF;
    buffer[2] = displaybuffer[2] >> 8;

    i2c_write_timeout_us(_i2c, SevSeg_ADDRESS, buffer, 3, false, 10000);
}
