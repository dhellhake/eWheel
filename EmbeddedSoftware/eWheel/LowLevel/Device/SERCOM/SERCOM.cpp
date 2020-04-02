/*
 * SERCOM.c
 *
 * Created: 15.03.2019 15:10:20
 *  Author: Dominik Hellhake
 */
#include "SERCOM.h"
#include "..\GCLK\GCLK.h"
#include "..\..\CC41A\CC41A.h"
#include "..\..\..\ESC\ESC.h"

void InitSERCOM0()
{
	//AT45DB
	//Set bits in the clock mask for an APBx bus.
	MCLK->APBCMASK.bit.SERCOM0_ = 1;
	
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[SERCOM0_GCLK_ID_CORE].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[SERCOM0_GCLK_ID_CORE].reg & GCLK_PCHCTRL_CHEN);

	/* Configure the peripheral channel */
	GCLK->PCHCTRL[SERCOM0_GCLK_ID_CORE].reg = GCLK_PCHCTRL_GEN(0);

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[SERCOM0_GCLK_ID_CORE].reg |= GCLK_PCHCTRL_CHEN;
	
	//Configure SERCOM0
	SERCOM0->SPI.CTRLA.reg =	(0x3 << SERCOM_SPI_CTRLA_MODE_Pos) |
								(0x1 << SERCOM_SPI_CTRLA_DOPO_Pos) |				//MOSI on PAD2; SCLK on PAD3
								(0x1 << SERCOM_SPI_CTRLA_DIPO_Pos) |				//MISO on PAD1
								(0x1 << SERCOM_SPI_CTRLA_CPHA_Pos);
	
	SERCOM0->SPI.CTRLB.reg =	(1 << SERCOM_SPI_CTRLB_RXEN_Pos);
	
	SERCOM0->SPI.BAUD.reg = (GetGCLK_Hz(SERCOM0_GCLK_ID_CORE) / (2 * 16000000)) - 1;
	
	SERCOM0->SPI.CTRLA.bit.ENABLE = 1;
	while(SERCOM0->SPI.SYNCBUSY.bit.ENABLE);
}
uint8_t SERCOM0_TransmitByte(uint8_t byte)
{
	while(SERCOM0->SPI.INTFLAG.bit.DRE == 0);
	SERCOM0->SPI.DATA.reg = byte;
	while(SERCOM0->SPI.INTFLAG.bit.RXC == 0);
	return SERCOM0->SPI.DATA.reg;
}

void InitSERCOM1()
{
	//USART: CC41A
	//921600 / 115200
	uint32_t baud_val = calculate_baud_value(230400 , GetGCLK_Hz(SERCOM1_GCLK_ID_CORE), 16);
	//Enable Clock for SERCOM5
	//Set bits in the clock mask for an APBx bus.
	MCLK->APBCMASK.bit.SERCOM1_ = 1;

	//Generate Clock al ref for BAUD
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[SERCOM1_GCLK_ID_CORE].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[SERCOM1_GCLK_ID_CORE].reg & GCLK_PCHCTRL_CHEN);

	/* Configure the peripheral channel */
	GCLK->PCHCTRL[SERCOM1_GCLK_ID_CORE].reg = GCLK_PCHCTRL_GEN(0); //Use Generic Clock Generator 0

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[SERCOM1_GCLK_ID_CORE].reg |= GCLK_PCHCTRL_CHEN;

	//Configure SERCOM5 for USART
	SERCOM1->USART.CTRLA.reg =  (1 << SERCOM_USART_CTRLA_DORD_Pos) |	// LSB first
								(1 << SERCOM_USART_CTRLA_RXPO_Pos) |	// Receive on PAD1 (PB03)
								(1 << SERCOM_USART_CTRLA_MODE_Pos);		// Use Internal Clock
	
	SERCOM1->USART.CTRLB.reg =  (1 << SERCOM_USART_CTRLB_RXEN_Pos) |	//1 = Receiver enabled
								(1 << SERCOM_USART_CTRLB_TXEN_Pos);		//1 = Transmit enable
	
	while(SERCOM1->USART.SYNCBUSY.bit.CTRLB);
	
	SERCOM1->USART.BAUD.reg = baud_val;
	
	NVIC_SetPriority(SERCOM1_IRQn, 1);
	NVIC->ISER[0] = (uint32_t)(1 << ((uint32_t)SERCOM1_IRQn & 0x0000001f));

	SERCOM1->USART.INTENSET.reg = SERCOM_USART_INTFLAG_RXC;
	SERCOM1->USART.CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;
	/* synchronization busy */
	while(SERCOM1->USART.SYNCBUSY.reg & SERCOM_USART_SYNCBUSY_ENABLE);
}
void SERCOM1_SendByte(uint8_t byte)
{
	while(!SERCOM1->USART.INTFLAG.bit.DRE);
	SERCOM1->USART.DATA.reg = byte;
}
void SERCOM1_Handler()
{
	if (SERCOM1->USART.INTFLAG.bit.RXC)
	{
		uint8_t rxData = SERCOM1->USART.DATA.reg;
		
		BLEDevice.ReceiveByte(rxData);
		
		return;
	}
}

void InitSERCOM2()
{	
	// LS9M
	//Enable Clock for SERCOM2
	//Set bits in the clock mask for an APBx bus.
	MCLK->APBCMASK.bit.SERCOM2_ = 1;
	
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[SERCOM2_GCLK_ID_CORE].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[SERCOM2_GCLK_ID_CORE].reg & GCLK_PCHCTRL_CHEN);

	/* Configure the peripheral channel */
	GCLK->PCHCTRL[SERCOM2_GCLK_ID_CORE].reg = GCLK_PCHCTRL_GEN(0);

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[SERCOM2_GCLK_ID_CORE].reg |= GCLK_PCHCTRL_CHEN;
	
	
	//Configure SERCOM0
	SERCOM2->SPI.CTRLA.reg =	(0x3 << SERCOM_SPI_CTRLA_MODE_Pos) |
								(0x1 << SERCOM_SPI_CTRLA_DOPO_Pos) |				//MOSI on PAD2 (PA10); SCLK on PAD3 (PA11)
								(0x1 << SERCOM_SPI_CTRLA_DIPO_Pos) |				//MISO on PAD 1 (PA09)
								(0x1 << SERCOM_SPI_CTRLA_CPHA_Pos);
	
	SERCOM2->SPI.CTRLB.reg =	(1 << SERCOM_SPI_CTRLB_RXEN_Pos);
	
	SERCOM2->SPI.BAUD.reg = (GetGCLK_Hz(SERCOM0_GCLK_ID_CORE) / (2 * 4000000)) - 1;
	
	SERCOM2->SPI.CTRLA.bit.ENABLE = 1;
	while(SERCOM2->SPI.SYNCBUSY.bit.ENABLE);
}
uint8_t SERCOM2_TransmitByte(uint8_t byte)
{
	while(SERCOM2->SPI.INTFLAG.bit.DRE == 0);
	SERCOM2->SPI.DATA.reg = byte;
	while(SERCOM2->SPI.INTFLAG.bit.RXC == 0);
	return SERCOM2->SPI.DATA.reg;
}

void InitSERCOM3()
{
	//USART:
	//VESC.RX => PA23 / SERCOM3.PAD0
	//VESC.TX => PA22 / SERCOM3.PAD1
	//230400
	uint32_t baud_val = calculate_baud_value(230400, GetGCLK_Hz(SERCOM3_GCLK_ID_CORE), 16);
	//Enable Clock for SERCOM3
	//Set bits in the clock mask for an APBx bus.
	MCLK->APBCMASK.bit.SERCOM3_ = 1;

	//Generate Clock al ref for BAUD
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[SERCOM3_GCLK_ID_CORE].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[SERCOM3_GCLK_ID_CORE].reg & GCLK_PCHCTRL_CHEN);

	/* Configure the peripheral channel */
	GCLK->PCHCTRL[SERCOM3_GCLK_ID_CORE].reg = GCLK_PCHCTRL_GEN(0); //Use Generic Clock Generator 0

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[SERCOM3_GCLK_ID_CORE].reg |= GCLK_PCHCTRL_CHEN;

	//Configure SERCOM5 for USART
	SERCOM3->USART.CTRLA.reg =  (1 << SERCOM_USART_CTRLA_DORD_Pos) |	// LSB first
								(1 << SERCOM_USART_CTRLA_RXPO_Pos) |	// Receive on PAD1 (PA23)
								(1 << SERCOM_USART_CTRLA_MODE_Pos);		// Use Internal Clock
	
	SERCOM3->USART.CTRLB.reg =  (1 << SERCOM_USART_CTRLB_RXEN_Pos) |	//1 = Receiver enabled
								(1 << SERCOM_USART_CTRLB_TXEN_Pos);		//1 = Transmit enable
	
	while(SERCOM3->USART.SYNCBUSY.bit.CTRLB);
	
	SERCOM3->USART.BAUD.reg = baud_val;
	
	NVIC_SetPriority(SERCOM3_IRQn, 1);
	NVIC->ISER[0] = (uint32_t)(1 << ((uint32_t)SERCOM3_IRQn & 0x0000001f));

	SERCOM3->USART.INTENSET.reg = SERCOM_USART_INTFLAG_RXC;
	SERCOM3->USART.CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;
	/* synchronization busy */
	while(SERCOM3->USART.SYNCBUSY.reg & SERCOM_USART_SYNCBUSY_ENABLE);
}
void SERCOM3_SendByte(uint8_t byte)
{
	while(!SERCOM3->USART.INTFLAG.bit.DRE);
	SERCOM3->USART.DATA.reg = byte;
}
void SERCOM3_Handler()
{
	if (SERCOM3->USART.INTFLAG.bit.RXC)
	{
		uint8_t rxData = SERCOM3->USART.DATA.reg;
		
		VESC.ReceiveByte(rxData);
		
		return;
	}
}

void InitSERCOM5()
{
	//USART:
	//CP2102.RX => PB02 / SERCOM5.PAD0 (MUX_D)
	//CP2102.TX => PB03 / SERCOM5.PAD1 (MUX_D)
	//921600 / 115200
	uint32_t baud_val = calculate_baud_value(921600 , GetGCLK_Hz(SERCOM5_GCLK_ID_CORE), 16);
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
	
	while(SERCOM5->USART.SYNCBUSY.bit.CTRLB);
	
	SERCOM5->USART.BAUD.reg = baud_val;
	
	NVIC_SetPriority(SERCOM5_IRQn, 1);
	NVIC->ISER[0] = (uint32_t)(1 << ((uint32_t)SERCOM5_IRQn & 0x0000001f));

	SERCOM5->USART.INTENSET.reg = SERCOM_USART_INTFLAG_RXC;
	SERCOM5->USART.CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;
	/* synchronization busy */
	while(SERCOM5->USART.SYNCBUSY.reg & SERCOM_USART_SYNCBUSY_ENABLE);
}
void SERCOM5_SendByte(uint8_t byte)
{
	while(!SERCOM5->USART.INTFLAG.bit.DRE);
	SERCOM5->USART.DATA.reg = byte;
}








