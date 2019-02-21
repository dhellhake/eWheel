/* 
* SSD1306.cpp
*
* Created: 30.05.2018 19:34:55
* Author: dominik hellhake
*/

#include "SSD1306.h"

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

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT SSD1306::Run()
{
	if (this->RowHasChanged[0])
	{
		this->WriteInt(this->RowValues[0], 0);
		this->RowHasChanged[0] = false;
		
		return RUN_RESULT::SUCCESS;
	}
	
	if (this->RowHasChanged[1])
	{
		this->WriteInt(this->RowValues[1], 1);
		this->RowHasChanged[1] = false;
		
		return RUN_RESULT::SUCCESS;
	}
	
	return RUN_RESULT::ERROR;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
SSD1306::SSD1306()
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
	
	this->InitDMADescriptors();
	
	uint8_t dataLength = 27;
	this->CMDBuffer[0] = 0x00;
	this->CMDBuffer[1] = 0xAE;			// DISPLAYOFF
	this->CMDBuffer[2] = 0xD5;			// SETDISPLAYCLOCKDIV
	this->CMDBuffer[3] = 0x80;
	this->CMDBuffer[4] = 0xA8;			// SETMULTIPLEX
	this->CMDBuffer[5] = 32 - 1;
	this->CMDBuffer[6] = 0xD3;			// SETDISPLAYOFFSET
	this->CMDBuffer[7] = 0x00;
	this->CMDBuffer[8] = 0x40 | 0x0;		// SETSTARTLINE
	this->CMDBuffer[9] = 0x8D;			// CHARGEPUMP
	this->CMDBuffer[10] = 0x14;
	this->CMDBuffer[11] = 0x20;			// MEMORYMODE
	this->CMDBuffer[12] = 0x01;
	this->CMDBuffer[13] = 0xA0 | 0x1;	// SEGREMAP
	this->CMDBuffer[14] = 0xC8;			// COMSCANDEC
	this->CMDBuffer[15] = 0xDA;			// SETCOMPINS
	this->CMDBuffer[16] = 0x02;
	this->CMDBuffer[17] = 0x81;			// SETCONTRAST
	this->CMDBuffer[18] = 0x8F;
	this->CMDBuffer[19] = 0xD9;			// SETPRECHARGE
	this->CMDBuffer[20] = 0xF1;
	this->CMDBuffer[21] = 0xDB;			// SETVCOMDETECT
	this->CMDBuffer[22] = 0x40;
	this->CMDBuffer[23] = 0xA4;			// DISPLAYALLON_RESUME
	this->CMDBuffer[24] = 0xA6;			// NORMALDISPLAY
	this->CMDBuffer[25] = 0x2E;			// DEACTIVATE_SCROLL
	this->CMDBuffer[26] = 0xAF;			// DISPLAYON
	
	
	/* Set transfer size, source address and destination address */
	this->DMAC_Descriptors[0].BTCNT.reg = dataLength;
	this->DMAC_Descriptors[0].SRCADDR.reg = (uint32_t)(this->CMDBuffer + dataLength);
		
	/* Set channel x descriptor 0 to the descriptor base address */
	System::StartDMATransfer(this->DMAC_Descriptors, 0);
	
	SERCOM4->I2CM.ADDR.reg =	SERCOM_I2CM_ADDR_ADDR(SLAVE_ADDR<<1) |
								SERCOM_I2CM_ADDR_LENEN |
								SERCOM_I2CM_ADDR_LEN(dataLength) |
								0;
								
	/* Wait for pending i2c-transfer */
	I2C::Wait();
}

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

void SSD1306::SetAddress(uint8_t row)
{	
	this->CMDBuffer[0] = 0x00;
	this->CMDBuffer[1] = 0x21;
	this->CMDBuffer[2] = 0x00;
	this->CMDBuffer[3] = 127;
	this->CMDBuffer[4] = 0x22;
	this->CMDBuffer[5] = (uint8_t)(row*2);
	this->CMDBuffer[6] = (uint8_t)((row*2) + 1);
		
	this->DMAC_Descriptors[0].BTCNT.reg = 7;								//Data-length = amount of bytes to send
	this->DMAC_Descriptors[0].SRCADDR.reg = (uint32_t)(this->CMDBuffer + 7);
	this->DMAC_Descriptors[0].DESCADDR.reg = 0;
	
	/* Set channel x descriptor 0 to the descriptor base address */
	System::StartDMATransfer(this->DMAC_Descriptors, 0);
	
	SERCOM4->I2CM.ADDR.reg =	SERCOM_I2CM_ADDR_ADDR(SLAVE_ADDR<<1) |
								SERCOM_I2CM_ADDR_LENEN |
								SERCOM_I2CM_ADDR_LEN(7) |
								0;
}

void SSD1306::WriteInt(uint32_t number, uint8_t row)
{	
	this->SetAddress(row);	
	
	/* Wait for pending i2c-transfer */
	I2C::Wait();
		
	/* Start I2C-Idle delay */
	uint32_t t1 = SysTick->VAL;
		
	/* Prepare CMD-PMAC-Descriptor */
	this->CMDBuffer[0] = 0x40;
	
	this->DMAC_Descriptors[0].BTCNT.reg = 1;
	this->DMAC_Descriptors[0].SRCADDR.reg = (uint32_t)(this->CMDBuffer + 1);
	this->DMAC_Descriptors[0].DESCADDR.reg = (uint32_t)&(this->DMAC_Descriptors[1]);
	
	uint8_t curDigit = 0;
	for (uint8_t x = 1; x < 9; x++)
	{		
		if (number > 0)
		{
			curDigit = number % 10;
			
			this->DMAC_Descriptors[x].SRCADDR.reg = (uint32_t)((((uint8_t*)SSD1306::ASCII + (32 * curDigit))) + 32);
			
			number = (number - curDigit) / 10;
		} else {
			this->DMAC_Descriptors[x].SRCADDR.reg = (uint32_t)(((uint8_t*)SSD1306::Blank) + 32);
		}
}

	/* Wait for rest of I2C-Idle delay */
	while ((t1 - 1900) < SysTick->VAL){}
	
	/* Set channel x descriptor 0 to the descriptor base address */
	System::StartDMATransfer(this->DMAC_Descriptors, 0);
	
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
		
		this->SetAddress(row);
		
		/* Wait for pending i2c-transfer */
		I2C::Wait();
		
		/* Start I2C-Idle delay */
		uint32_t t1 = SysTick->VAL;
				
		/* Prepare CMD-PMAC-Descriptor */
		this->CMDBuffer[0] = 0x40;
		
		this->DMAC_Descriptors[0].BTCNT.reg = 1;
		this->DMAC_Descriptors[0].SRCADDR.reg = (uint32_t)(this->CMDBuffer + 1);
		this->DMAC_Descriptors[0].DESCADDR.reg = (uint32_t)&(this->DMAC_Descriptors[1]);
		
		for (uint8_t x = 1; x < 9; x++)
			this->DMAC_Descriptors[x].SRCADDR.reg = (uint32_t)(((uint8_t*)SSD1306::Blank) + 32);
		
		/* Wait for rest of I2C-Idle delay */
		while ((t1 - 1900) < SysTick->VAL){}
		
		/* Set channel x descriptor 0 to the descriptor base address */
		System::StartDMATransfer(this->DMAC_Descriptors, 0);
		
		SERCOM4->I2CM.ADDR.reg =	SERCOM_I2CM_ADDR_ADDR(SLAVE_ADDR<<1) |
									SERCOM_I2CM_ADDR_LENEN |
									SERCOM_I2CM_ADDR_LEN(255) |
									0;					
	}	
}
