/*
 * SysTick.c
 *
 * Created: 15.03.2019 15:49:54
 *  Author: Dominik Hellhake
 */
#include "SysTick.h"

// Overflow of 16bit-Systick-Counter running at 48Mhz
volatile uint64_t SysTick_Overflow = 0;

void InitSysTick()
{
	SysTick->CTRL = 0;					// Disable SysTick
	SysTick->LOAD = 0xFFFFFF;			// Set reload register for overflow interrupts
	NVIC_SetPriority(SysTick_IRQn, 3);	// Set interrupt priority to least urgency
	SysTick->VAL = 0;					// Reset the SysTick counter value
	SysTick->CTRL = 0x00000007;			// Enable SysTick, Enable SysTick Exceptions, Use CPU Clock
	NVIC_EnableIRQ(SysTick_IRQn);		// Enable SysTick Interrupt
}

void SysTick_Handler(void)
{
	SysTick_Overflow++;
}