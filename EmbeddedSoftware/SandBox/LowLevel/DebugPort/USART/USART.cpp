/* 
* USART.cpp
*
* Created: 01.05.2018 17:57:33
* Author: dominik hellhake
*/


#include "USART.h"

// static class member
bool USART::isInitialized = false;

// default constructor
USART::USART()
{
} //USART

// default constructor
void USART::InitSERCOM()
{
	if (USART::isInitialized)
		return;
	
	//USART:
	//CP2102.RX => PB02 / SERCOM5.PAD0 (MUX_D)
	//CP2102.TX => PB03 / SERCOM5.PAD1 (MUX_D)
	
	uint32_t baud_val = calculate_baud_value(115200, get_gclk_hz(SERCOM5_GCLK_ID_CORE), 16);
	//Enable Clock for SERCOM5
	//Set bits in the clock mask for an APBx bus.
	MCLK->APBCMASK.bit.SERCOM5_ = 1;

	//Generate Clock al ref for BAUD
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[SERCOM5_GCLK_ID_CORE].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[SERCOM5_GCLK_ID_CORE].reg & GCLK_PCHCTRL_CHEN);

	/* Configure the peripheral channel */
	GCLK->PCHCTRL[SERCOM5_GCLK_ID_CORE].reg = GCLK_PCHCTRL_GEN(0); //Use Generic Clock Generator 0

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[SERCOM5_GCLK_ID_CORE].reg |= GCLK_PCHCTRL_CHEN;

	//Configure SERCOM5 for USART
	SERCOM5->USART.CTRLA.reg =  (1 << SERCOM_USART_CTRLA_DORD_Pos) |	// LSB first
								(1 << SERCOM_USART_CTRLA_RXPO_Pos) |	// Receive on PAD1 (PB03)
								(1 << SERCOM_USART_CTRLA_MODE_Pos);		// Use Internal Clock
	
	SERCOM5->USART.CTRLB.reg =  (1 << SERCOM_USART_CTRLB_RXEN_Pos) |	//1 = Receiver enabled
								(1 << SERCOM_USART_CTRLB_TXEN_Pos);		//1 = Transmit enable
	
	while(SERCOM3->USART.SYNCBUSY.bit.CTRLB);
	
	SERCOM5->USART.BAUD.reg = baud_val;
	
	pin_set_peripheral_function(PINMUX_PB02D_SERCOM5_PAD0);
	pin_set_peripheral_function(PINMUX_PB03D_SERCOM5_PAD1);


	NVIC->ISER[0] = (uint32_t)(1 << ((uint32_t)SERCOM5_IRQn & 0x0000001f));

	SERCOM5->USART.INTENSET.reg = SERCOM_USART_INTFLAG_RXC;
	SERCOM5->USART.CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;
	/* synchronization busy */
	while(SERCOM5->USART.SYNCBUSY.reg & SERCOM_USART_SYNCBUSY_ENABLE);
	
	USART::isInitialized = true;
} //USART


void USART::SendByte(uint8_t byte)
{
	while(!SERCOM5->USART.INTFLAG.bit.DRE);
	SERCOM5->USART.DATA.reg = byte;
}

void SERCOM5_Handler()
{
	
	
}

// default destructor
USART::~USART()
{
} //~USART
