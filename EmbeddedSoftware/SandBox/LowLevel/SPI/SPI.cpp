/*
 * SPI.cpp
 *
 * Created: 30.04.2018 15:48:21
 *  Author: dominik hellhake
 */ 

#include "SPI.h"

// default constructor
SPIPort::SPIPort()
{
} //SPIPort

// default destructor
SPIPort::~SPIPort()
{
} //~SPIPort

void SPIPort::InitSERCOM()
{
	//Enable Clock for SERCOM0
	//Set bits in the clock mask for an APBx bus.
	MCLK->APBCMASK.bit.SERCOM0_ = 1;
	
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[SERCOM0_GCLK_ID_CORE].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[SERCOM0_GCLK_ID_CORE].reg & GCLK_PCHCTRL_CHEN);

	/* Configure the peripheral channel */
	GCLK->PCHCTRL[SERCOM0_GCLK_ID_CORE].reg = GCLK_PCHCTRL_GEN(0);

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[SERCOM0_GCLK_ID_CORE].reg |= GCLK_PCHCTRL_CHEN;
	
	System::SetPinPeripheralFunction(PINMUX_PA09C_SERCOM0_PAD1);
	System::SetPinPeripheralFunction(PINMUX_PA10C_SERCOM0_PAD2);
	System::SetPinPeripheralFunction(PINMUX_PA11C_SERCOM0_PAD3);
	
	//Configure SERCOM0	
	SERCOM0->SPI.CTRLA.reg =	(0x3 << SERCOM_SPI_CTRLA_MODE_Pos) |
								(0x1 << SERCOM_SPI_CTRLA_DOPO_Pos) |				//MOSI on PAD2 (PA10); SCLK on PAD3 (PA11)
								(0x1 << SERCOM_SPI_CTRLA_DIPO_Pos) |				//MISO on PAD 1 (PA09)
								(0x1 << SERCOM_SPI_CTRLA_CPHA_Pos);
								
	SERCOM0->SPI.CTRLB.reg =	(1 << SERCOM_SPI_CTRLB_RXEN_Pos);
		
	SERCOM0->SPI.BAUD.reg = (System::GetGCLK_Hz(SERCOM0_GCLK_ID_CORE) / (2 * 4000000)) - 1;
	
	SERCOM0->SPI.CTRLA.bit.ENABLE = 1;
	while(SERCOM0->SPI.SYNCBUSY.bit.ENABLE);
}