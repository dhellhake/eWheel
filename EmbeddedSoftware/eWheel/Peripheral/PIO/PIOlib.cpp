/* 
* PIOlib.cpp
*
* Created: 13.05.2020 21:05:11
* Author: dominik hellhake
*/
#include "PIOlib.h"
#include "..\..\LSM9DS1\LSM9DS1.h"

void InitPIO()
{	
	/* SPI Pins */
	// Disable interrupts on the pin PB0
	PIOA->PIO_IDR = PIO_PA11 | PIO_PA12 | PIO_PA13 | PIO_PA14;
	// Select Peripheral function A
	PIOA->PIO_ABCDSR[0] &= ~(PIO_PA11 | PIO_PA12 | PIO_PA13 | PIO_PA14);
	PIOA->PIO_ABCDSR[1] &= ~(PIO_PA11 | PIO_PA12 | PIO_PA13 | PIO_PA14);
	PIOA->PIO_PDR = PIO_PA11 | PIO_PA12 | PIO_PA13 | PIO_PA14;	
	
	
	/* Configure and enable interrupt of PIOB. */
	NVIC_DisableIRQ(PIOB_IRQn);
	NVIC_ClearPendingIRQ(PIOB_IRQn);
	NVIC_SetPriority(PIOB_IRQn, 4);
	NVIC_EnableIRQ(PIOB_IRQn);
}

void PIOB_Handler()
{
	uint32_t pio_mask = PIOB->PIO_ISR;
	return;
	if ((pio_mask & PIO_PB0) != 0x0)
		Gyro.ReadValues();
}