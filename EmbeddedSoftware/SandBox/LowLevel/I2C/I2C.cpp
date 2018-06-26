/* 
* I2C.cpp
*
* Created: 30.05.2018 18:19:17
* Author: dominik hellhake
*/


#include "I2C.h"

uint16_t I2C::tx_index = 0;
uint16_t I2C::tx_count = 0;
uint8_t *I2C::buffer_ref = 0x00;

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
	SERCOM4->I2CM.BAUD.reg = SERCOM_I2CM_BAUD_BAUD(40);
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
	
	/* Init I2C-DMAC channel */
	I2C::InitDMAC();
}

void I2C::InitDMAC()
{	
	DMAC->CHID.reg = DMAC_CHID_ID(0);
	DMAC->CHCTRLA.reg &= ~DMAC_CHCTRLA_ENABLE;
	DMAC->CHCTRLA.reg = DMAC_CHCTRLA_SWRST;
	DMAC->CHCTRLA.reg |= DMAC_CHCTRLA_RUNSTDBY;
	
	DMAC->SWTRIGCTRL.reg &= (uint32_t)(~(1 << 0));
	
	DMAC->CHCTRLB.reg =	DMAC_CHCTRLB_LVL(0) | \
						DMAC_CHCTRLB_TRIGSRC(SERCOM4_DMAC_ID_TX) | \
						DMAC_CHCTRLB_TRIGACT(DMAC_CHCTRLB_TRIGACT_BEAT_Val);	
	
	/* Enable DMAC interrupt */
	NVIC->ISER[0] = (uint32_t)(1 << ((uint32_t)DMAC_IRQn & 0x0000001f));
	DMAC->CHINTENSET.reg = DMAC_CHINTENSET_TERR | DMAC_CHINTENSET_TCMPL | DMAC_CHINTENSET_SUSP;
}

void I2C::StartDMACTransfer(DmacDescriptor *descriptor)
{
	System::StartDMATransfer(descriptor, 0);
}

// default destructor
I2C::~I2C()
{
} //~I2C
