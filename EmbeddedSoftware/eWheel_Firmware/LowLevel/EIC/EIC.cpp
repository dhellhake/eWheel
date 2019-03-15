/*
 * EIC.c
 *
 * Created: 15.03.2019 15:56:17
 *  Author: Dominik Hellhake
 */ 
#include "EIC.h"
#include "..\System\System.h"

void InitEIC()
{
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[EIC_GCLK_ID].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[EIC_GCLK_ID].reg & GCLK_PCHCTRL_CHEN);

	/* Configure the peripheral channel */
	GCLK->PCHCTRL[EIC_GCLK_ID].reg = GCLK_PCHCTRL_GEN(0);

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[EIC_GCLK_ID].reg |= GCLK_PCHCTRL_CHEN;
	
	// Reset EIC
	EIC->CTRLA.reg = EIC_CTRLA_SWRST;
	while (EIC->CTRLA.reg & EIC_CTRLA_SWRST);
	
	
	NVIC_SetPriority(EIC_IRQn, 0);
	NVIC->ISER[0] = (uint32_t)(1 << ((uint32_t)EIC_IRQn & 0x0000001f));
	
	EIC->CONFIG[0].reg =	EIC_CONFIG_SENSE2_RISE |						//LSM6D-INT1
	EIC_CONFIG_SENSE7_BOTH;							//Hall Sensor
	EIC->CONFIG[1].reg =	EIC_CONFIG_SENSE4_BOTH	|
	EIC_CONFIG_SENSE5_BOTH;
	
	EIC->CTRLA.reg = EIC_CTRLA_ENABLE;
}

void EIC_Handler()
{
	if ((EIC->INTFLAG.reg & ((1 << 7) | (1 << 12) | (1 << 13))) != 0x00)
	{
		/* Hall Sensor caused interrupt */
		eWheel.BLDCSensor.Run();
		eWheel.BLDCController.Drive_SetPhase(eWheel.BLDCSensor.HallState);
		eWheel.BLDCSensor.Propagate();
		
		EIC->INTFLAG.reg = (1 << 7) | (1 << 12) | (1 << 13);
	}
}