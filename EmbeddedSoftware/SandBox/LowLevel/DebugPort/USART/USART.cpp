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
	SERCOM_USART_CTRLA_Type ctrla;
	ctrla.bit.DORD = 1;			//LSB first
	ctrla.bit.CPOL = 0;			//Rising XCK edge
	ctrla.bit.CMODE = 0;			//Async communication
	ctrla.bit.FORM = 0x0;		//USAT frame with parity
	ctrla.bit.SAMPA = 0x0;		//7-8-9 / 3-4-5
	ctrla.bit.RXPO = 0x1;		//Receive on PAD1 (PB03)
	ctrla.bit.TXPO = 0x0;		//Transmit on PAD0 (PB02)
	ctrla.bit.SAMPR = 0x0;		//16x over-sampling on arithmetic baud
	ctrla.bit.IBON = 0; 		//Interupt on data-stream
	ctrla.bit.RUNSTDBY = 0;		//Generic Clock disable on finish
	ctrla.bit.MODE = 0x1;			//Use Internal Clock
	SERCOM5->USART.CTRLA.reg = ctrla.reg;
	
	
	SERCOM_USART_CTRLB_Type ctrlb;
	ctrlb.bit.LINCMD = 0x0;		//0x0 = Normal USART
	ctrlb.bit.RXEN = 1;			//1 = Receiver enabled
	ctrlb.bit.TXEN = 1;			//1 = Transmit enable
	ctrlb.bit.PMODE = 0;			//0 = Even parity
	ctrlb.bit.ENC = 0;			//0 = no encoding
	ctrlb.bit.SFDE = 0;			//0 = Start-of-frame detection disabled
	ctrlb.bit.COLDEN = 0;		//0 = collision detect disabled
	ctrlb.bit.SBMODE = 0;		//0 = one stop bit
	ctrlb.bit.CHSIZE = 0x0;		//0x0 = 8 bits
	SERCOM5->USART.CTRLB.reg = ctrlb.reg;
	
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
