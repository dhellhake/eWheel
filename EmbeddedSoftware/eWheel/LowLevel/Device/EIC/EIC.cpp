/*
 * EIC.cpp
 *
 * Created: 22.03.2020 16:38:22
 *  Author: Dominik Hellhake
 */ 
#include "EIC.h"
#include "..\..\..\LSM9D\LSM9D.h"

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
		
	EIC->CONFIG[1].reg =	EIC_CONFIG_SENSE5_RISE;							//Interrupt on rise for LSM6D-INT1
	
	EIC->CTRLA.reg = EIC_CTRLA_ENABLE;
}

void EIC_Handler()
{
	if ((EIC->INTFLAG.reg & (1 << 13)) != 0x00)
	{
		/* LSM9D->INT1 caused interrupt */
		Gyro.ReadValues();
		
		EIC->INTFLAG.reg = (1 << 13);
	}
}