/*
 * SPI.c
 *
 * Created: 09.06.2019 14:30:19
 *  Author: Dominik Hellhake
 */
#include "SPI.h"

void init_spi(void)
{
	// Set MOSI, SCK as Output
	DDRB= (1 << 5) | (1 << 3) | (1 << 2);
	
	// Enable SPI, Set as Master
	// Prescaler: Fosc/16, Enable Interrupts
	//The MOSI, SCK pins are as per ATMega8
	SPCR =	(1 << SPE) |
			(1 << MSTR) |
			(0 << SPR0) |
			(0 << SPR1);
	
	SPSR =	(1 << SPI2X);
}

uint8_t spi_putc (uint8_t data)
{
	// Load data into the buffer
	SPDR = data;
	
	//Wait until transmission complete
	while(!(SPSR & (1<<SPIF) ));
	
	// Return received data
	return(SPDR);
}