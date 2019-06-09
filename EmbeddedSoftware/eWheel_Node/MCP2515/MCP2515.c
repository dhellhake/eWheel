/*
 * MCP2515.c
 *
 * Created: 09.06.2019 14:32:16
 *  Author: Dominik Hellhake
 */
#include "MCP2515.h"
#include "..\SPI\SPI.h"

void mcp2515_init()
{
	init_spi();	
	
}

uint8_t mcp2515_read_register(uint8_t adress)
{
	uint8_t data;
	
	PORTB &= ~(1 << 2);
	
	spi_putc(0b00000011);
	spi_putc(adress);
	data = spi_putc(0xff);
	
	PORTB |= (1 << 2);
	
	return data;
}