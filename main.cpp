/*
 *  Title: main
 *  Author: Tinna Osk
 */

// Includes
#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/i2c.h>
#include <BME280.h>
// Pinouts
const uint8_t PIN_BME_SDA = 2;
const uint8_t PIN_BME_SCL = 3;

// Constructors
BME280 bme;

void init() {
    stdio_init_all();
    i2c_init(i2c1, 400000);
    gpio_set_function(PIN_BME_SDA, GPIO_FUNC_I2C);
    gpio_set_function(PIN_BME_SCL, GPIO_FUNC_I2C);
    bme = BME280(i2c1, BME280_alt_I2Caddr);

    sleep_ms(5000);
    
    printf("Initializing BME280... ");
    if(!bme.init()) {
        printf("Failed to initialize!\n");
        while(true); // Stop here
    } else {
        printf("Initialized.\n");
    }
}

void loop() {
    bme.read();
    printf("Pressure: %.2f [hPa] \n", bme.pressure);
    printf("Temperature: %.2f [°C] \n", bme.temperature);
    printf("Humidity: %.2f [%%rh] \n \n", bme.humidity);
    sleep_ms(2000);
}

int main() {
    init();
    while(1) {
        loop();
    }
    return 0;
}