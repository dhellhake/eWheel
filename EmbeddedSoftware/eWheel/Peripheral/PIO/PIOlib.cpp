/* 
* PIOlib.cpp
*
* Created: 13.05.2020 21:05:11
* Author: dominik hellhake
*/
#include "PIOlib.h"

void InitPIO()
{	
	/* SPI Pins */
	// Disable interrupts on the pin PB0
	PIOA->PIO_IDR = PIO_PA11 | PIO_PA12 | PIO_PA13 | PIO_PA14;
	// Select Peripheral function A
	PIOA->PIO_ABCDSR[0] &= ~(PIO_PA11 | PIO_PA12 | PIO_PA13 | PIO_PA14);
	PIOA->PIO_ABCDSR[1] &= ~(PIO_PA11 | PIO_PA12 | PIO_PA13 | PIO_PA14);
	PIOA->PIO_PDR = PIO_PA11 | PIO_PA12 | PIO_PA13 | PIO_PA14;
	
	/* USART0 Pins */
	// Disable interrupts on the pin PB0
	PIOA->PIO_IDR = PIO_PA5 | PIO_PA6;
	// Select Peripheral function A
	PIOA->PIO_ABCDSR[0] &= ~(PIO_PA5 | PIO_PA6);
	PIOA->PIO_ABCDSR[1] &= ~(PIO_PA5 | PIO_PA6);
	PIOA->PIO_PDR = PIO_PA5 | PIO_PA6;
}
