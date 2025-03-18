/**
 *  @filename   :   epd7in5.cpp
 *  @brief      :   Implements for e-paper library
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

#include <stdlib.h>
#include <iostream>
#include "epd7in5_V2.h"

Epd::~Epd() {
};

Epd::Epd() {
    reset_pin = RST_PIN;
    dc_pin = DC_PIN;
    cs_pin = CS_PIN;
    busy_pin = BUSY_PIN;
    width = EPD_WIDTH;
    height = EPD_HEIGHT;
    spi = NULL;
};

int Epd::Init(spi_inst_t *spi_inst) {
    spi = spi_inst;
    if (IfInit(spi) != 0) {
        return -1;
    }

    Reset();

	SendCommand(0x01);                      // POWER SETTING
	SendData(0x07);                         // border disabled, internal power
    SendData(0x07);                         // VGH=20V VGL=-20V             
	SendData(0x3f);                         // VDH=15V
	SendData(0x3f);                         // VDL=-15V

	SendCommand(0x06);                      // Booster Setting
	SendData(0x17);
	SendData(0x17);
	SendData(0x28);
	SendData(0x17);

    SendCommand(0x04);                      //POWER ON
    DelayMs(100);
    WaitUntilIdle();

    SendCommand(0X00);	                    //PANNEL SETTING
    SendData(0x1F);                         //LUT from OTP, black/white KW mode, scan up, shift right, booster on, soft reset

    SendCommand(0x61);                      //RESOLUTION SETTING
    SendData(0x03);		                    //source 800
    SendData(0x20);
    SendData(0x01);		                    //gate 480
    SendData(0xE0);

    SendCommand(0X15);                      //DUAL SPI MODE
    SendData(0x00);                         //disabled

    SendCommand(0X50);                      //VCOM AND DATA INTERVAL SETTING
    SendData(0x10);
    SendData(0x07);

    SendCommand(0X60);	                    //TCON SETTING
    SendData(0x22);

    return 0;
}

int Epd::InitPart(void) {
    Reset();
    
    SendCommand(0x00);
    SendData(0x1F);

    SendCommand(0x04); //POWER ON
	DelayMs(100); 
	WaitUntilIdle();

    return 0;
}

int Epd::Init4Gray(void) {
    Reset();

	SendCommand(0x01);  // POWER SETTING
	SendData(0x07);     // border disabled, internal power
    SendData(0x07);     // VGH=20V VGL=-20V             
	SendData(0x3f);     // VDH=15V
	SendData(0x3f);     // VDL=-15V

	SendCommand(0x06);  // Booster Setting
	SendData(0x27);
	SendData(0x27);
	SendData(0x18);
	SendData(0x17);

    SendCommand(0x04);  //POWER ON
    DelayMs(100);
    WaitUntilIdle();

    SendCommand(0X00);	//PANNEL SETTING
    SendData(0x1F);     //LUT from OTP, black/white KW mode, scan up, shift right, booster on, soft reset

    SendCommand(0x61);  //RESOLUTION SETTING
    SendData(0x03);		//source 800
    SendData(0x20);
    SendData(0x01);	    //gate 480
    SendData(0xE0);

    SendCommand(0X15);  //DUAL SPI MODE
    SendData(0x00);     //disabled

    SendCommand(0X50);  //VCOM AND DATA INTERVAL SETTING
    SendData(0x10);
    SendData(0x07);

    SendCommand(0X60);	//TCON SETTING
    SendData(0x22);

    return 0;
}

/**
 *  @brief: basic function for sending commands
 */
void Epd::SendCommand(const uint8_t command) {
    uint8_t msg[1];
    msg[0] = command;
    DigitalWrite(dc_pin, 0);
    SpiTransfer(spi, msg, 1);
}

/**
 *  @brief: basic function for sending data
 */
void Epd::SendData(const uint8_t data) {
    uint8_t msg[1];
    msg[0] = data; 
    DigitalWrite(dc_pin, 1);
    SpiTransfer(spi, msg, 1);
}

void Epd::SendBuffer(const uint8_t *buffer, size_t len) {
    DigitalWrite(dc_pin, 1);
    SpiTransfer(spi, buffer, len);
}

/**
 *  @brief: Wait until the busy_pin goes HIGH
 */
void Epd::WaitUntilIdle(void) {
    unsigned char busy;
    std::cout << "e-Paper Busy" << std::endl;
    do{
        SendCommand(0x71);
        busy = DigitalRead(busy_pin);
    }while(busy == 0);
    std::cout << "e-Paper Busy Release" << std::endl;
    DelayMs(20);
}

/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see Epd::Sleep();
 */
void Epd::Reset(void) {
    DigitalWrite(reset_pin, 1);
    DelayMs(20); 
    DigitalWrite(reset_pin, 0);                //module reset    
    DelayMs(4);
    DigitalWrite(reset_pin, 1);
    DelayMs(20);    
}

/**
 *  @brief: display a fullscreen image
 */
void Epd::DisplayFrame(const unsigned char* frame_buffer) {
    
    SendCommand(0x10);
    for (unsigned long j = 0; j < height; j++) {
        for (unsigned long i = 0; i < width/8; i++) {
            SendData(frame_buffer[i + j * width/8]);
        }
    }

    SendCommand(0x13);
    for (unsigned long j = 0; j < height; j++) {
        for (unsigned long i = 0; i < width/8; i++) {
            SendData(~frame_buffer[i + j * width/8]);
        }
    }

    SendCommand(0x12);
    DelayMs(100);
    WaitUntilIdle();
}

/**
 *  @brief: display a partial image
 */
void Epd::DisplayPart(const unsigned char* pbuffer, unsigned long xStart, unsigned long yStart, unsigned long xEnd, unsigned long yEnd) {

    uint16_t partWidth = ((xEnd - xStart) % 8 == 0)?((xEnd - xStart) / 8):((xEnd - xStart)/ 8 + 1);
    uint16_t partHeight = yEnd - yStart;

    SendCommand(0x50);
	SendData(0xA9);
	SendData(0x07);

	SendCommand(0x91);		//This command makes the display enter partial mode
	SendCommand(0x90);		//resolution setting
	SendData (xStart/256);
	SendData (xStart%256);  //x-start    

	SendData (xEnd/256);		
	SendData (xEnd%256-1);  //x-end	

	SendData (yStart/256);  
	SendData (yStart%256);  //y-start    

	SendData (yEnd/256);		
	SendData (yEnd%256-1);  //y-end
	SendData (0x01);

    SendCommand(0x13);
    for (uint16_t j=0; j < partHeight; j++) {
        SendBuffer((uint8_t *)(pbuffer+j*partWidth), partWidth);
    }

    // SendCommand(0x13);
    // for (unsigned long j = 0; j < height; j++) {
    //     for (unsigned long i = 0; i < width/8; i++) {
    //         if( (j>=yStart) && (j<yStart+Picture_Height) && (i*8>=xStart) && (i*8<xStart+Picture_Width)){
    //             SendData(~pbuffer[i-xStart/8 + (Picture_Width)/8*(j-yStart)]);
    //         }else {
    //             SendData(0x00);
    //         }
    //     }
    // }

    SendCommand(0x12);
    DelayMs(100);
    WaitUntilIdle();

    SendCommand(0x92);
}

// void Epd::SetLut_by_host(unsigned char* lut_vcom,  unsigned char* lut_ww, unsigned char* lut_bw, unsigned char* lut_wb, unsigned char* lut_bb)
// {
// 	unsigned char count;

// 	SendCommand(0x20); //VCOM	
// 	for(count=0; count<42; count++)
// 		SendData(lut_vcom[count]);

// 	SendCommand(0x21); //LUTBW
// 	for(count=0; count<42; count++)
// 		SendData(lut_ww[count]);

// 	SendCommand(0x22); //LUTBW
// 	for(count=0; count<42; count++)
// 		SendData(lut_bw[count]);

// 	SendCommand(0x23); //LUTWB
// 	for(count=0; count<42; count++)
// 		SendData(lut_wb[count]);

// 	SendCommand(0x24); //LUTBB
// 	for(count=0; count<42; count++)
// 		SendData(lut_bb[count]);
// }

/**
 *  @brief: After this command is transmitted, the chip would enter the 
 *          deep-sleep mode to save power. 
 *          The deep sleep mode would return to standby by hardware reset. 
 *          The only one parameter is a check code, the command would be
 *          executed if check code = 0xA5. 
 *          You can use EPD_Reset() to awaken
 */
void Epd::Sleep(void) {
    SendCommand(0X02);
    WaitUntilIdle();
    SendCommand(0X07);
    SendData(0xA5);
}

void Epd::Clear(void) {

    SendCommand(0x10);
    for(unsigned long i=0; i<height*width; i++)	{
        SendData(0xFF);
    }

    SendCommand(0x13);
    for(unsigned long i=0; i<height*width; i++)	{
        SendData(0x00);
    }
    SendCommand(0x12);
    DelayMs(100);
    WaitUntilIdle();
}


/* END OF FILE */


