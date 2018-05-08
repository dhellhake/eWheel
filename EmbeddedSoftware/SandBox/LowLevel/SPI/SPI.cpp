/*
 * SPI.cpp
 *
 * Created: 30.04.2018 15:48:21
 *  Author: dominik hellhake
 */ 

#include "SPI.h"

// static class member
bool SPIPort::isInitialized = false;

// default constructor
SPIPort::SPIPort()
{
} //USART

// default destructor
SPIPort::~SPIPort()
{
} //~USART

void SPIPort::InitSERCOM()
{
	if (SPIPort::isInitialized)
		return;
	
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
	
	System::SetPinPeripheralFunction(PINMUX_PA08C_SERCOM0_PAD0);
	System::SetPinPeripheralFunction(PINMUX_PA10C_SERCOM0_PAD2);
	System::SetPinPeripheralFunction(PINMUX_PA11C_SERCOM0_PAD3);
	
	//Configure SERCOM0	
	SERCOM0->SPI.CTRLA.reg =	(0x3 << SERCOM_SPI_CTRLA_MODE_Pos) |
								(1 << SERCOM_SPI_CTRLA_DOPO_Pos);
								
	SERCOM0->SPI.CTRLB.reg =	(1 << SERCOM_SPI_CTRLB_RXEN_Pos);
	
	
	SERCOM0->SPI.BAUD.reg = (System::GetGCLK_Hz(SERCOM0_GCLK_ID_CORE) / (2 * 10000000)) - 1;
	
	NVIC->ISER[0] = (uint32_t)(1 << ((uint32_t)SERCOM0_IRQn & 0x0000001f));
	
	SERCOM0->SPI.CTRLA.bit.ENABLE = 1;
	while(SERCOM0->SPI.SYNCBUSY.bit.ENABLE);
	
	SPIPort::isInitialized = true;	
}

void SERCOM0_Handler()
{
	
}