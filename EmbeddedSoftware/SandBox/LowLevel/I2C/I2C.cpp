/* 
* I2C.cpp
*
* Created: 30.05.2018 18:19:17
* Author: domin
*/


#include "I2C.h"

uint8_t I2C::tx_index = 0;
uint16_t I2C::tx_count = 0;
uint8_t *I2C::buffer_ref = &I2C::tx_index;
	
volatile bool I2C::tx_complete = true;

// default constructor
I2C::I2C()
{
} //I2C

void I2C::InitSERCOM()
{
	//Enable Clock for SERCOM4
	//Set bits in the clock mask for an APBx bus.
	MCLK->APBCMASK.bit.SERCOM4_ = 1;
	
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[SERCOM4_GCLK_ID_CORE].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[SERCOM4_GCLK_ID_CORE].reg & GCLK_PCHCTRL_CHEN);

	/* Configure the peripheral channel */
	GCLK->PCHCTRL[SERCOM4_GCLK_ID_CORE].reg = GCLK_PCHCTRL_GEN(0);

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[SERCOM4_GCLK_ID_CORE].reg |= GCLK_PCHCTRL_CHEN;
	
	
	PORT->Group[1].OUTSET.reg = PORT_PB08;
	PORT->Group[1].PINCFG[PIN_PB08].reg = PORT_PINCFG_PULLEN;
	System::SetPinPeripheralFunction(PINMUX_PB08D_SERCOM4_PAD0);
	
	PORT->Group[1].PINCFG[PIN_PB09].reg = PORT_PINCFG_PULLEN;
	PORT->Group[1].OUTSET.reg = PORT_PB09;
	System::SetPinPeripheralFunction(PINMUX_PB09D_SERCOM4_PAD1);
	
	
	SERCOM4->I2CM.CTRLA.reg =	(0x0 << SERCOM_I2CM_CTRLA_SPEED_Pos) |				//Select fast mode for 400kHz
								(0x5 << SERCOM_I2CM_CTRLA_MODE_Pos)	;				//Do run in master mode
	/* smart mode enabled by setting the bit SMEN as 1 */
	SERCOM4->I2CM.CTRLB.reg = SERCOM_I2CM_CTRLB_SMEN;
	/* synchronization busy */
	while(SERCOM4->I2CM.SYNCBUSY.bit.SYSOP);
	/* BAUD = 120 for 400kHz-SCL @ 48Mhz GCLK */
	SERCOM4->I2CM.BAUD.reg = SERCOM_I2CM_BAUD_BAUD(120);
	/* synchronization busy */
	while(SERCOM4->I2CM.SYNCBUSY.bit.SYSOP);
	/* SERCOM4 peripheral enabled by setting the ENABLE bit as 1*/
	SERCOM4->I2CM.CTRLA.reg |= SERCOM_I2CM_CTRLA_ENABLE;
	/* SERCOM Enable synchronization busy */
	while((SERCOM4->I2CM.SYNCBUSY.reg & SERCOM_I2CM_SYNCBUSY_ENABLE));
	/* bus state is forced into idle state */
	SERCOM4->I2CM.STATUS.bit.BUSSTATE = 0x1;
	/* synchronization busy */
	while(SERCOM4->I2CM.SYNCBUSY.bit.SYSOP);
	/* Both master on bus and slave on bus interrupt is enabled */
	SERCOM4->I2CM.INTENSET.reg = SERCOM_I2CM_INTENSET_MB | SERCOM_I2CM_INTENSET_SB;
	/* SERCOM4 handler enabled */
	NVIC->ISER[0] = (uint32_t)(1 << ((uint32_t)SERCOM4_IRQn & 0x0000001f));	
}

void I2C::StartTransmitt(uint8_t slave_addr, uint8_t *bufferRef, uint16_t count)
{	
	I2C::tx_count = count;
	I2C::buffer_ref = bufferRef;
	I2C::tx_index = 0;
	
	I2C::tx_complete = false;
		
	/* Acknowledge section is set as ACK signal by
		writing 0 in ACKACT bit */
	SERCOM4->I2CM.CTRLB.reg &= ~SERCOM_I2CM_CTRLB_ACKACT;
	while(SERCOM4->I2CM.SYNCBUSY.bit.SYSOP);
	/* slave address with Write(0) */
	SERCOM4->I2CM.ADDR.reg = (slave_addr << 1) | 0;
}

void SERCOM4_Handler(void)
{
	I2C::Handler();
}

// default destructor
I2C::~I2C()
{
} //~I2C
