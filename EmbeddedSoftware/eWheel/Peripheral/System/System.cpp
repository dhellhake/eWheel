/* 
* System.cpp
*
* Created: 29.04.2020 21:30:28
* Author: dominik hellhake
*/
#include "System.h"

volatile uint32_t ElapsedMillis = 0;

void InitSysTick()
{
	/* Configure SysTick Counter */
	SysTick->LOAD  =	(uint32_t)((SYSTEM_CPU_CLK / 1000) - 1UL);			/* set reload register */
	NVIC_SetPriority	(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);	/* set Priority for Systick Interrupt */
	SysTick->VAL   =	0UL;												/* Load the SysTick Counter Value */
	SysTick->CTRL  =	SysTick_CTRL_CLKSOURCE_Msk |						/* Use Processor Clock */
						SysTick_CTRL_TICKINT_Msk   |						/* Interrupt enable */
						SysTick_CTRL_ENABLE_Msk;							/* SysTick enable */	
}

void SysTick_Handler()
{
	ElapsedMillis++;
}