/* 
* System.cpp
*
* Created: 29.04.2020 21:30:28
* Author: dominik hellhake
*/
#include "System.h"

// Overflow of 24bit-Systick-Counter running at 120Mhz
volatile uint64_t SysTick_Overflow = 0;

void InitSysTick()
{
	/* Configure SysTick Counter */
	SysTick->LOAD  =	0xFFFFFF;											/* set reload register */
	NVIC_SetPriority	(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);	/* set Priority for Systick Interrupt */
	SysTick->VAL   =	0UL;												/* Load the SysTick Counter Value */
	SysTick->CTRL  =	SysTick_CTRL_CLKSOURCE_Msk |						/* Use Processor Clock */
						SysTick_CTRL_TICKINT_Msk   |						/* Interrupt enable */
						SysTick_CTRL_ENABLE_Msk;							/* SysTick enable */	
}

void SysTick_Handler()
{
	SysTick_Overflow++;
}