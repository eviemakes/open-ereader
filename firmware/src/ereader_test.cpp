// cmake -DPICO_BOARD=adafruit_feather_rp2040 ..

#include <iostream>
#include "pico/stdlib.h"
#include "lib/display/epd7in5_V2.h"
#include "lib/display/imagedata.h"

using namespace std;

const uint8_t SQUARE[10] = {0xFF};

int main() {

    spi_inst_t *spi = spi0;
    stdio_init_all();

    Epd epd;
    cout << "e-Paper init" << endl;
    if (epd.Init(spi) != 0) {
        cout << "e-Paper init failed" << endl;
        return 0;
    }
    
    cout << "e-Paper Display" << endl;
    //epd.DisplayFrame(gImage_7in5_V2);
    
    //sleep_ms(5000);
    epd.InitPart();
    //epd.DisplayPart(IMAGE_DATA, 250, 200, 250+240, 200+103);
    epd.DisplayPart(SQUARE, 160, 100, 320, 101);

    epd.Init(spi);
    cout << "e-Paper Clear" << endl;
    epd.Clear();

    epd.Sleep();

    return 0;
}