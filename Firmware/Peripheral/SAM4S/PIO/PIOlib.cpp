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
	// Disable interrupts
	PIOA->PIO_IDR = PIO_PA9 | PIO_PA11 | PIO_PA12 | PIO_PA13 | PIO_PA14;
	// Select Peripheral function A
	PIOA->PIO_ABCDSR[0] = PIO_PA9;
	PIOA->PIO_ABCDSR[1] = 0x00000000;	
	PIOA->PIO_PDR = PIO_PA9 | PIO_PA11 | PIO_PA12 | PIO_PA13 | PIO_PA14;
	
	/* USART0 Pins */
	// Disable interrupts 
	PIOA->PIO_IDR = PIO_PA5 | PIO_PA6;
	// Select Peripheral function A
	PIOA->PIO_ABCDSR[0] &= ~(PIO_PA5 | PIO_PA6);
	PIOA->PIO_ABCDSR[1] &= ~(PIO_PA5 | PIO_PA6);
	PIOA->PIO_PDR = PIO_PA5 | PIO_PA6;
		
	/* USART1 Pins */
	// Disable interrupts 
	PIOA->PIO_IDR = PIO_PA21 | PIO_PA22;
	// Select Peripheral function A
	PIOA->PIO_ABCDSR[0] &= ~(PIO_PA21 | PIO_PA22);
	PIOA->PIO_ABCDSR[1] &= ~(PIO_PA21 | PIO_PA22);
	PIOA->PIO_PDR = PIO_PA21 | PIO_PA22;
		
	/* UART1 Pins */
	// Disable interrupts 
	PIOB->PIO_IDR = PIO_PB2 | PIO_PB3;
	// Select Peripheral function A
	PIOB->PIO_ABCDSR[0] &= ~(PIO_PB2 | PIO_PB3);
	PIOB->PIO_ABCDSR[1] &= ~(PIO_PB2 | PIO_PB3);
	PIOB->PIO_PDR = PIO_PB2 | PIO_PB3;
}
