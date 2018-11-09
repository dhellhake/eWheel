/* 
* SSD1306.cpp
*
* Created: 30.05.2018 19:34:55
* Author: dominik hellhake
*/


#include "SSD1306.h"

DmacDescriptor SSD1306::DMAC_Descriptors[9];

uint8_t SSD1306::Blank[32] = {
	0, 0,
	0, 0,
	0, 0,
	0, 0,
	0, 0,
	0, 0,
	0, 0,
	0, 0,
	0, 0,
	0, 0,
	0, 0,
	0, 0,
	0, 0,
	0, 0,
	0, 0,
	0, 0
};

uint8_t SSD1306::CMDBuffer[27] = {};

uint8_t SSD1306::ASCII[320] = {
	0, 0,
	7, 192,
	15, 224,
	31, 240,
	56, 112,
	56, 24,
	76, 8,
	70, 8,
	67, 8,
	65, 136,
	64, 200,
	96, 112,
	48, 112,
	63, 224,
	31, 192,
	15, 128,

	0, 0,
	0, 0,
	0, 8,
	0, 8,
	0, 8,
	0, 8,
	0, 8,
	127, 248,
	127, 248,
	127, 248,
	64, 8,
	32, 8,
	32, 8,
	32, 8,
	16, 8,
	0, 0,

	0, 0,
	0, 0,
	56, 8,
	56, 8,
	124, 8,
	102, 8,
	70, 8,
	67, 8,
	65, 8,
	65, 136,
	64, 200,
	64, 72,
	64, 120,
	96, 56,
	32, 24,
	0, 0,

	0, 0,
	0, 0,
	0, 224,
	25, 240,
	61, 240,
	125, 16,
	103, 24,
	70, 8,
	66, 8,
	66, 8,
	66, 8,
	66, 8,
	66, 8,
	32, 24,
	32, 16,
	0, 0,

	0, 0,
	0, 64,
	0, 64,
	0, 64,
	0, 64,
	127, 248,
	127, 248,
	127, 248,
	120, 64,
	60, 64,
	30, 64,
	14, 64,
	7, 64,
	3, 192,
	1, 192,
	0, 192,

	0, 0,
	0, 0,
	1, 192,
	65, 224,
	67, 240,
	67, 48,
	70, 24,
	70, 8,
	68, 8,
	68, 8,
	68, 8,
	68, 8,
	124, 8,
	124, 8,
	124, 16,
	0, 0,

	0, 0,
	1, 192,
	3, 224,
	67, 240,
	70, 56,
	68, 24,
	68, 8,
	68, 8,
	68, 8,
	98, 8,
	34, 24,
	49, 240,
	63, 240,
	31, 224,
	7, 192,
	0, 0,

	0, 0,
	96, 0,
	112, 0,
	112, 0,
	88, 0,
	76, 0,
	78, 0,
	71, 0,
	67, 0,
	65, 192,
	64, 248,
	64, 120,
	64, 56,
	64, 0,
	64, 0,
	0, 0,

	0, 0,
	0, 0,
	0, 96,
	56, 240,
	61, 240,
	125, 152,
	71, 8,
	66, 8,
	66, 8,
	70, 8,
	71, 8,
	125, 152,
	61, 248,
	24, 240,
	0, 96,
	0, 0,

	0, 0,
	15, 128,
	31, 224,
	63, 224,
	62, 48,
	97, 16,
	65, 24,
	64, 136,
	64, 136,
	64, 136,
	64, 136,
	96, 136,
	49, 136,
	63, 8,
	31, 0,
	14, 0
};

void SSD1306::InitDMADescriptors()
{
	for (uint8_t x = 0; x < 9; x++)
	{
		/* Set block transfer control to base descriptor */
		SSD1306::DMAC_Descriptors[x].BTCTRL.bit.VALID = true;
		SSD1306::DMAC_Descriptors[x].BTCTRL.bit.EVOSEL = 0;
		SSD1306::DMAC_Descriptors[x].BTCTRL.bit.BLOCKACT = 0;
		SSD1306::DMAC_Descriptors[x].BTCTRL.bit.BEATSIZE = 0;
		SSD1306::DMAC_Descriptors[x].BTCTRL.bit.SRCINC = true;
		SSD1306::DMAC_Descriptors[x].BTCTRL.bit.DSTINC = false;
		SSD1306::DMAC_Descriptors[x].BTCTRL.bit.STEPSEL = 0;
		SSD1306::DMAC_Descriptors[x].BTCTRL.bit.STEPSIZE = 0;
		SSD1306::DMAC_Descriptors[x].BTCNT.reg = 32;
		
		SSD1306::DMAC_Descriptors[x].DSTADDR.reg = (uint32_t)(&SERCOM4->I2CM.DATA.reg);
		
		/* Set next transfer descriptor address */
		if (x > 0 && x < 8)
			SSD1306::DMAC_Descriptors[x].DESCADDR.reg = (uint32_t)&(SSD1306::DMAC_Descriptors[x + 1]);
		else
		{
			SSD1306::DMAC_Descriptors[x].BTCNT.reg = 30;
			SSD1306::DMAC_Descriptors[x].DESCADDR.reg = 0;
		}
	}
}

void SSD1306::Init()
{
	// ERRATA: Swap ASCII Bit-Pattern for each character.
	uint8_t tmp;
	for (uint16_t x = 0; x < 320; x += 2)
	{
		tmp = SSD1306::ASCII[x+1];
		SSD1306::ASCII[x+1] = SSD1306::ASCII[x];
		SSD1306::ASCII[x] = tmp;
	}
	
	/* Init I2C peripheral */
	I2C::InitSERCOM();
	
	SSD1306::InitDMADescriptors();
	
	uint8_t dataLength = 27;
	SSD1306::CMDBuffer[0] = 0x00;
	SSD1306::CMDBuffer[1] = 0xAE;			// DISPLAYOFF
	SSD1306::CMDBuffer[2] = 0xD5;			// SETDISPLAYCLOCKDIV
	SSD1306::CMDBuffer[3] = 0x80;
	SSD1306::CMDBuffer[4] = 0xA8;			// SETMULTIPLEX
	SSD1306::CMDBuffer[5] = 32 - 1;
	SSD1306::CMDBuffer[6] = 0xD3;			// SETDISPLAYOFFSET
	SSD1306::CMDBuffer[7] = 0x00;
	SSD1306::CMDBuffer[8] = 0x40 | 0x0;		// SETSTARTLINE
	SSD1306::CMDBuffer[9] = 0x8D;			// CHARGEPUMP
	SSD1306::CMDBuffer[10] = 0x14;
	SSD1306::CMDBuffer[11] = 0x20;			// MEMORYMODE
	SSD1306::CMDBuffer[12] = 0x01;
	SSD1306::CMDBuffer[13] = 0xA0 | 0x1;	// SEGREMAP
	SSD1306::CMDBuffer[14] = 0xC8;			// COMSCANDEC
	SSD1306::CMDBuffer[15] = 0xDA;			// SETCOMPINS
	SSD1306::CMDBuffer[16] = 0x02;
	SSD1306::CMDBuffer[17] = 0x81;			// SETCONTRAST
	SSD1306::CMDBuffer[18] = 0x8F;
	SSD1306::CMDBuffer[19] = 0xD9;			// SETPRECHARGE
	SSD1306::CMDBuffer[20] = 0xF1;
	SSD1306::CMDBuffer[21] = 0xDB;			// SETVCOMDETECT
	SSD1306::CMDBuffer[22] = 0x40;
	SSD1306::CMDBuffer[23] = 0xA4;			// DISPLAYALLON_RESUME
	SSD1306::CMDBuffer[24] = 0xA6;			// NORMALDISPLAY
	SSD1306::CMDBuffer[25] = 0x2E;			// DEACTIVATE_SCROLL
	SSD1306::CMDBuffer[26] = 0xAF;			// DISPLAYON
	
	
	/* Set transfer size, source address and destination address */
	SSD1306::DMAC_Descriptors[0].BTCNT.reg = dataLength;
	SSD1306::DMAC_Descriptors[0].SRCADDR.reg = (uint32_t)(SSD1306::CMDBuffer + dataLength);
		
	/* Set channel x descriptor 0 to the descriptor base address */
	System::StartDMATransfer(SSD1306::DMAC_Descriptors, 0);
	
	SERCOM4->I2CM.ADDR.reg =	SERCOM_I2CM_ADDR_ADDR(SLAVE_ADDR<<1) |
								SERCOM_I2CM_ADDR_LENEN |
								SERCOM_I2CM_ADDR_LEN(dataLength) |
								0;
}

void SSD1306::SetAddress(uint8_t row)
{	
	SSD1306::CMDBuffer[0] = 0x00;
	SSD1306::CMDBuffer[1] = 0x21;
	SSD1306::CMDBuffer[2] = 0x00;
	SSD1306::CMDBuffer[3] = 127;
	SSD1306::CMDBuffer[4] = 0x22;
	SSD1306::CMDBuffer[5] = (uint8_t)(row*2);
	SSD1306::CMDBuffer[6] = (uint8_t)((row*2) + 1);
		
	SSD1306::DMAC_Descriptors[0].BTCNT.reg = 7;								//Data-length = amount of bytes to send
	SSD1306::DMAC_Descriptors[0].SRCADDR.reg = (uint32_t)(SSD1306::CMDBuffer + 7);
	SSD1306::DMAC_Descriptors[0].DESCADDR.reg = 0;
	
	/* Set channel x descriptor 0 to the descriptor base address */
	System::StartDMATransfer(SSD1306::DMAC_Descriptors, 0);
	
	SERCOM4->I2CM.ADDR.reg =	SERCOM_I2CM_ADDR_ADDR(SLAVE_ADDR<<1) |
								SERCOM_I2CM_ADDR_LENEN |
								SERCOM_I2CM_ADDR_LEN(7) |
								0;
}

void SSD1306::WriteInt(uint32_t number, uint8_t row)
{	
	SSD1306::SetAddress(row);
	
	
	/* Wait for pending i2c-transfer */
	I2C::Wait();
		
	/* Start I2C-Idle delay */
	uint32_t t1 = SysTick->VAL;
		
	/* Prepare CMD-PMAC-Descriptor */
	SSD1306::CMDBuffer[0] = 0x40;
	
	SSD1306::DMAC_Descriptors[0].BTCNT.reg = 1;
	SSD1306::DMAC_Descriptors[0].SRCADDR.reg = (uint32_t)(SSD1306::CMDBuffer + 1);
	SSD1306::DMAC_Descriptors[0].DESCADDR.reg = (uint32_t)&(SSD1306::DMAC_Descriptors[1]);
	
	uint8_t curDigit = 0;
	for (uint8_t x = 1; x < 9; x++)
	{		
		if (number > 0)
		{
			curDigit = number % 10;
			
			SSD1306::DMAC_Descriptors[x].SRCADDR.reg = (uint32_t)((((uint8_t*)SSD1306::ASCII + (32 * curDigit))) + 32);
			
			number = (number - curDigit) / 10;
		} else {
			SSD1306::DMAC_Descriptors[x].SRCADDR.reg = (uint32_t)(((uint8_t*)SSD1306::Blank) + 32);
		}
}

	/* Wait for rest of I2C-Idle delay */
	while ((t1 - 1900) < SysTick->VAL){}
	
	/* Set channel x descriptor 0 to the descriptor base address */
	System::StartDMATransfer(SSD1306::DMAC_Descriptors, 0);
	
	SERCOM4->I2CM.ADDR.reg =	SERCOM_I2CM_ADDR_ADDR(SLAVE_ADDR<<1) |
								SERCOM_I2CM_ADDR_LENEN |
								SERCOM_I2CM_ADDR_LEN(255) |
								0;														
}


void SSD1306::Clear()
{
	for (uint8_t row = 0; row < 2; row++)
	{		
		/* Wait for pending i2c-transfer */
		I2C::Wait();	
		
		SSD1306::SetAddress(row);
		
		/* Wait for pending i2c-transfer */
		I2C::Wait();
		
		/* Start I2C-Idle delay */
		uint32_t t1 = SysTick->VAL;
				
		/* Prepare CMD-PMAC-Descriptor */
		SSD1306::CMDBuffer[0] = 0x40;
		
		SSD1306::DMAC_Descriptors[0].BTCNT.reg = 1;
		SSD1306::DMAC_Descriptors[0].SRCADDR.reg = (uint32_t)(SSD1306::CMDBuffer + 1);
		SSD1306::DMAC_Descriptors[0].DESCADDR.reg = (uint32_t)&(SSD1306::DMAC_Descriptors[1]);
		
		for (uint8_t x = 1; x < 9; x++)
			SSD1306::DMAC_Descriptors[x].SRCADDR.reg = (uint32_t)(((uint8_t*)SSD1306::Blank) + 32);
		
		/* Wait for rest of I2C-Idle delay */
		while ((t1 - 1900) < SysTick->VAL){}
		
		/* Set channel x descriptor 0 to the descriptor base address */
		System::StartDMATransfer(SSD1306::DMAC_Descriptors, 0);
		
		SERCOM4->I2CM.ADDR.reg =	SERCOM_I2CM_ADDR_ADDR(SLAVE_ADDR<<1) |
									SERCOM_I2CM_ADDR_LENEN |
									SERCOM_I2CM_ADDR_LEN(255) |
									0;
					
	}	
}
