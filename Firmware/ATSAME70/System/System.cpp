/*
* System.cpp
*
* Created: 29.04.2020 21:30:28
* Author: dominik hellhake
*/
#include "System.h"

Sys System;

Sys::Sys()
{
	/* Configure SysTick Counter */
	SysTick->LOAD  =	0xFFFFFF;											/* set reload register */
	NVIC_SetPriority	(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);	/* set Priority for Systick Interrupt */
	SysTick->VAL   =	0UL;												/* Load the SysTick Counter Value */
	SysTick->CTRL  =	(1 << SysTick_CTRL_CLKSOURCE_Pos) |						/* Use Processor Clock */
						(1 << SysTick_CTRL_TICKINT_Pos)   |						/* Interrupt enable */
						(1 << SysTick_CTRL_ENABLE_Pos);							/* SysTick enable */
}

void SysTick_Handler()
{	
	System.SysTick_Overflow++;
}