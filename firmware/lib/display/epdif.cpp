/**
 *  @filename   :   epdif.cpp
 *  @brief      :   Implements EPD interface functions
 *                  Users have to implement all the functions in epdif.cpp
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     August 10 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "epdif.h"

EpdIf::EpdIf() {
};

EpdIf::~EpdIf() {
};

void EpdIf::DigitalWrite(int pin, int value) {
    gpio_put(pin, value);
}

int EpdIf::DigitalRead(int pin) {
    return gpio_get(pin);
}

void EpdIf::DelayMs(unsigned int delaytime) {
    sleep_ms(delaytime);
}

void EpdIf::SpiTransfer(spi_inst_t *spi, const uint8_t *data, size_t len) {
    gpio_put(CS_PIN, 0);
    spi_write_blocking(spi, data, len);
    gpio_put(CS_PIN, 1);
}

int EpdIf::IfInit(spi_inst_t *spi) {

    gpio_init(BUSY_PIN);
    gpio_set_dir(BUSY_PIN, GPIO_IN);
    gpio_init(RST_PIN);
    gpio_set_dir(RST_PIN, GPIO_OUT);
    gpio_init(DC_PIN);
    gpio_set_dir(DC_PIN, GPIO_OUT);

    gpio_init(CS_PIN);
    gpio_set_dir(CS_PIN, GPIO_OUT);
    gpio_put(CS_PIN, 1);

    //pinMode(PWR_PIN, OUTPUT);
    //DigitalWrite(PWR_PIN, 1);

    spi_init(spi, 2000000);
    spi_set_format(spi0, 8, SPI_CPOL_1, SPI_CPHA_1, SPI_MSB_FIRST);
    
    gpio_set_function(SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(MOSI_PIN, GPIO_FUNC_SPI);

    return 0;
}

