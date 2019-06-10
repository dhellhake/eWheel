/*
 * MCP2515.c
 *
 * Created: 09.06.2019 14:32:16
 *  Author: Dominik Hellhake
 */
#include "..\System.h"
#include <util/delay.h>
 
#include "MCP2515.h"
#include "..\SPI\SPI.h"


void mcp2515_RTS()
{	
	PORTB &= ~(1 << 2);
	
	spi_putc(0b10000001);
	
	PORTB |= (1 << 2);
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
void mcp2515_read_register_cont(uint8_t adress, uint8_t* data, uint8_t length)
{	
	PORTB &= ~(1 << 2);
	
	spi_putc(0b00000011);
	spi_putc(adress);
	
	for (uint8_t index = 0; index < length; index++)
		data[index] = spi_putc(0xff);
	
	PORTB |= (1 << 2);
}

void mcp2515_write_register(uint8_t adress, uint8_t data)
{
	PORTB &= ~(1 << 2);
	
	spi_putc(0b00000010);
	spi_putc(adress);
	spi_putc(data);
	
	PORTB |= (1 << 2);
}
void mcp2515_write_register_cont(uint8_t adress, uint8_t* data, uint8_t length)
{
	PORTB &= ~(1 << 2);
	
	spi_putc(0b00000010);
	spi_putc(adress);
	
	for (uint8_t index = 0; index < length; index++)
		spi_putc(data[index]);
	
	PORTB |= (1 << 2);
}


void mcp2515_bit_modify(uint8_t adress, uint8_t mask, uint8_t data)
{
	PORTB &= ~(1 << 2);
	
	spi_putc(0b00000101);
	spi_putc(adress);
	spi_putc(mask);
	spi_putc(data);
	
	PORTB |= (1 << 2);
}

void mcp2515_reset()
{
	PORTB &= ~(1 << 2);
	
	spi_putc(0b11000000);
	
	PORTB |= (1 << 2);
}

uint8_t mcp2515_status()
{
	uint8_t data;
	
	PORTB &= ~(1 << 2);
	
	spi_putc(0b10010000);
	data = spi_putc(0xff);
	
	PORTB |= (1 << 2);
	
	return data;	
}

uint8_t mcp2515_read_rx_status(void)
{
	uint8_t data;
	
	PORTB &= ~(1 << 2);
	
	spi_putc(RX_STATUS);
	data = spi_putc(0xff);
	
	PORTB |= (1 << 2);
	
	return data;
}

void mcp2515_init()
{	
	// External Interrupt to MCP2515
	DDRD &= ~(1 << DDD2);
	PORTD |= (1 << PORTD2);
	EICRA |= (0 << ISC00) | (0 << ISC01);    
	EIMSK |= (1 << INT0);     // Turns on INT0
	
	init_spi();
	
	mcp2515_reset();
	_delay_ms(1);
	
	mcp2515_write_register(CNF1, 0);
	mcp2515_write_register(CNF2, 0x88);
	mcp2515_write_register(CNF3, 3);
	
	mcp2515_write_register(CANINTE, (1 << RX1IE) | (1 << RX0IE));
	
	mcp2515_write_register(RXB0CTRL, (1 << RXM1) | (1 << RXM0));
	mcp2515_write_register(RXB1CTRL, (1 << RXM1) | (1 << RXM0));
	
	mcp2515_bit_modify(CANCTRL, 0xE0, 0);
}

void mcp2515_send_message(CANMessage *p_message)
{
	uint8_t data[13] = { 0 };
	
	data[0] = (uint8_t)(p_message->id >> 3);	//TXB0SIDH
	data[1] = (uint8_t)(p_message->id << 5);	//TXB0SIDL
	data[2] = 0x00;								//TXB0EID8
	data[3] = 0x00;								//TXB0EID0
	data[4] = p_message->length;				//TXB0DLC
	
	//TXB0D0-7
	for (uint8_t index = 0; index < p_message->length; index++)
	data[5 + index] = p_message->data[index];
	
	mcp2515_write_register_cont(0b00110001, data, 13);
	mcp2515_RTS();
}