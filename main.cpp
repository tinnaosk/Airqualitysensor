/*
 *  Title: main
 *  Author: Tinna Osk
 */

// Includes
#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/i2c.h>
#include <SEN55.h>
#include <SCD30.h>
#include <LMP91.h>
#include <MCP3564R.h>
#include <SevSeg.h>

// Pinouts
const uint8_t PIN_BME_SDA = 2;
const uint8_t PIN_BME_SCL = 3;

// Constructors
SEN55 sen55;
SCD30 scd30;
LMP91 lmp91(i2c1, 0);
MCP3564R mcp3564r(spi1, 1);

SEN55_VALUES values;
float temp, co2, hum;

uint8_t channel;
int32_t raw_no2;

#define DISPLAY_ADDRESS_1 0x70
#define DISPLAY_ADDRESS_2 0x71
#define DISPLAY_ADDRESS_3 0x72
#define DISPLAY_ADDRESS_4 0x73
#define DISPLAY_ADDRESS_5 0x74

SevSeg temp_display = SevSeg(i2c1, DISPLAY_ADDRESS_5);
SevSeg no2_display = SevSeg(i2c1, DISPLAY_ADDRESS_4);
SevSeg co2_display = SevSeg(i2c1, DISPLAY_ADDRESS_3);
SevSeg pm10_display = SevSeg(i2c1, DISPLAY_ADDRESS_2);
SevSeg pm1_display = SevSeg(i2c1, DISPLAY_ADDRESS_1);

union packet_t {
    struct {
        float temperature;
        float no2;
        uint16_t humidity;
        uint16_t co2;
        uint16_t pm10;
        uint16_t pm4;
        uint16_t pm2_5;
        uint16_t pm1;
    };
    uint8_t raw[20];
} __attribute__((packed, aligned(sizeof(uint32_t))));

packet_t packet;

void init() {
    stdio_init_all();
    i2c_init(i2c1, 400000);
    spi_init(spi1, 10000000u);
    spi_set_format(spi1, 8, spi_cpol_t::SPI_CPOL_0, spi_cpha_t::SPI_CPHA_0, spi_order_t::SPI_MSB_FIRST);
    gpio_set_function(14, GPIO_FUNC_SPI);
    gpio_set_function(15, GPIO_FUNC_SPI);
    gpio_set_function(8, GPIO_FUNC_SPI);
    gpio_set_function(PIN_BME_SDA, GPIO_FUNC_I2C);
    gpio_set_function(PIN_BME_SCL, GPIO_FUNC_I2C);
    sen55 = SEN55(i2c1);
    scd30 = SCD30(i2c1);

    sleep_ms(5000);
    
    printf("Initializing SEN55... ");
    if(!sen55.init()) {
        printf("Failed to initialize!\n");
        while(true); // Stop here
    } else {
        printf("Initialized.\n");
    }

        printf("Initializing SCD30... ");
    if(!scd30.init()) {
        printf("Failed to initialize!\n");
        while(true); // Stop here
    } else {
        printf("Initialized.\n");
    }

    //LMP91000
    lmp91.init();
    lmp91.set_REF_SOURCE(REF_SOURCE::Source_external);
    lmp91.set_R_LOAD(R_LOAD::Load_100);
    lmp91.set_TIA_GAIN(TIA_GAIN::Gain_350k);
    lmp91.set_OP_MODE(OP_MODE::Mode_3_Lead_Amperpmetric);
    printf("LMP Initialized\n");

    //MCP3564R
    mcp3564r.init();
    mcp3564r.set_clock_source(2);
    mcp3564r.select_vref_source(false);
    mcp3564r.set_data_format(3);
    mcp3564r.enable_scan_channel(0);
    mcp3564r.set_adc_gain(1);
    mcp3564r.set_oversample_ratio(9);
    mcp3564r.set_conv_mode(3);
    mcp3564r.set_adc_mode(3);
    printf("MCP initialized\n");

    temp_display.begin();
    no2_display.begin();
    co2_display.begin();
    pm10_display.begin();
    pm1_display.begin();
    
    temp_display.setBrightness(5);
    no2_display.setBrightness(5);
    co2_display.setBrightness(5);
    pm10_display.setBrightness(5);
    pm1_display.setBrightness(5);
    printf("Displays initialized\n");
}

void loop() {
    mcp3564r.read_data(&raw_no2, &channel);
    if(sen55.read(&values)) {
    printf("SEN55: \n");
    printf("PM 1: %f\n", values.pm1);
    printf("PM 2.5: %f\n", values.pm2_5);
    printf("PM 4: %f\n", values.pm4);
    printf("PM 10: %f\n", values.pm10);
    printf("RH %: %f\n", values.RH);
    printf("Temperature: %f\n", values.temp);
    } else {
        printf("Failed to read from SEN55\n");
    }
    if (scd30.read()) {
        printf("SCD30: \n");
        printf("CO2: %f\n", scd30.co2);
        printf("RH: %f\n", scd30.hum);
        printf("Temperature: %f\n", scd30.temp);
    } else {
        printf("Failed to read from SCD30\n");
    }

    // -18 nA (per ppm) * 350k (TIA gain) * 1 (ADC gain) * 2.048V (ref) / 2^24 (bits)
    float no2_ppm = ((float)raw_no2) * 7.69042969e-10f;
    printf("NO2: %f\n", no2_ppm);

    packet.temperature = scd30.temp;
    packet.humidity = scd30.hum;
    packet.no2 = no2_ppm;
    packet.co2 = scd30.co2;
    packet.pm10 = values.pm10;
    packet.pm4 = values.pm4;
    packet.pm2_5 = values.pm2_5;
    packet.pm1 = values.pm1;
    
    temp_display.clear();
    no2_display.clear();
    co2_display.clear();
    pm10_display.clear();
    pm1_display.clear();

    temp_display.printFloat((double)packet.temperature, 1, 10);
    no2_display.printFloat((double)packet.no2, 3, 10);
    co2_display.printNumber(packet.co2, 10);
    pm10_display.printNumber(packet.pm10, 10);
    pm1_display.printNumber(packet.pm1, 10);

    temp_display.writeDisplay();
    no2_display.writeDisplay();
    co2_display.writeDisplay();
    pm10_display.writeDisplay();
    pm1_display.writeDisplay();
    
    sleep_ms(2500);
}

int main() {
    init();
    while(1) {
        loop();
    }
    return 0;
}