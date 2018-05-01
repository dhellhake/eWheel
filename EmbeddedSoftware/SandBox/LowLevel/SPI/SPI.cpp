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
} //USART

// default destructor
SPIPort::~SPIPort()
{
} //~USART

void SPIPort::SPIInit()
{
	// max 10Mhz
	// MSB first
	// CPOL=0 (~SERCOM_SPI_CTRLA_CPOL)
	// CPHA=0 (~SERCOM_SPI_CTRLA_CPHA)
	// Module SERCOM0
	// PINMUX_PA08C_SERCOM0_PAD0
	// PINMUX_PA09C_SERCOM0_PAD1
	// PINMUX_PA10C_SERCOM0_PAD2
	// PINMUX_PA11C_SERCOM0_PAD3

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
	
	pin_set_peripheral_function(PINMUX_PA08C_SERCOM0_PAD0);
	pin_set_peripheral_function(PINMUX_PA10C_SERCOM0_PAD2);
	pin_set_peripheral_function(PINMUX_PA11C_SERCOM0_PAD3);
	
	//Configure SERCOM0
	SERCOM_SPI_CTRLA_Type ctrla;
	ctrla.bit.DORD = 0;		//MSB first
	ctrla.bit.CPHA = 0;		//CPHA mode 0
	ctrla.bit.CPOL = 0;		//CPOL mode 0
	ctrla.bit.FORM = 0;		//SPI frame
	ctrla.bit.DIPO = 0;		//MISO on PAD0
	ctrla.bit.DOPO = 1;		//MOSI on PAD2; SCK on PAD3; SS on PAD 1
	ctrla.bit.MODE = 3;		//Master mode	
	SERCOM0->SPI.CTRLA.reg = ctrla.reg;
	
	SERCOM_SPI_CTRLB_Type ctrlb;
	ctrlb.bit.RXEN = 1;   // Receive is enabled
	ctrlb.bit.MSSEN = 0;  // Software controlled Slave Select
	ctrlb.bit.CHSIZE = 0;  // 8-Bit character transfer	
	SERCOM0->SPI.CTRLB.reg = ctrlb.reg;
	
	SERCOM0->SPI.BAUD.reg = (get_gclk_hz(SERCOM0_GCLK_ID_CORE) / (2 * 10000000)) - 1;
	
	NVIC->ISER[0] = (uint32_t)(1 << ((uint32_t)SERCOM0_IRQn & 0x0000001f));
	
	SERCOM0->SPI.CTRLA.bit.ENABLE = 1;
	while(SERCOM0->SPI.SYNCBUSY.bit.ENABLE);	
}

uint8_t SPIPort::SendByte(uint8_t byte)
{
	while(SERCOM0->SPI.INTFLAG.bit.DRE == 0);
	SERCOM0->SPI.DATA.reg = byte;
	while(SERCOM0->SPI.INTFLAG.bit.RXC == 0);
	return SERCOM0->SPI.DATA.reg;
}