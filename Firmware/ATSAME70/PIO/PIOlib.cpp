/* 
* PIOlib.cpp
*
* Created: 13.05.2020 21:05:11
* Author: dominik hellhake
*/
#include "PIOlib.h"

void InitPIO()
{	
	/* HSMCI Pins */
	// MCCK  -> PA25 D
	// MCCDA -> PA28 C
	// MCDA0 -> PA30 C
	// MCDA1 -> PA31 C
	// MCDA2 -> PA26 C
	// MCDA3 -> PA27 C
	// Disable interrupts
	PIOA->PIO_IDR = PIO_PA25 | PIO_PA26 | PIO_PA27 | PIO_PA28 | PIO_PA30 | PIO_PA31;
	// Select Peripheral function C
	PIOA->PIO_ABCDSR[0] = PIO_PA25;
	PIOA->PIO_ABCDSR[1] = PIO_PA25 | PIO_PA26 | PIO_PA27 | PIO_PA28 | PIO_PA30 | PIO_PA31;
	PIOA->PIO_PDR = PIO_PA25 | PIO_PA26 | PIO_PA27 | PIO_PA28 | PIO_PA30 | PIO_PA31;
}
