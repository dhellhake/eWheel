/*
 * MCP2515.h
 *
 * Created: 09.06.2019 14:32:22
 *  Author: Dominik Hellhake
 */
#ifndef MCP2515_H_
#define MCP2515_H_

#include <avr/io.h>

#define CANINTF		0x2C
#define RX0IF		0x00

#define	RX_STATUS	0xB0

#define CANINTE		0x2B
#define RX1IE		0x01
#define RX0IE		0x00

#define CNF1		0x2A
#define BRP0		0x00

#define CNF2		0x29
#define BTLMODE		0x07
#define PHSEG11		0x04

#define CNF3		0x28
#define PHSEG21		0x01

#define RXB0CTRL	0x60
#define RXB1CTRL	0x70
#define RXM1		0x06
#define RXM0		0x05

#define CANCTRL		0x0F

typedef struct
{
	uint16_t  id;
	uint8_t   rtr;
	uint8_t   length;
	uint8_t   data[8];
} CANMessage;

void mcp2515_init();
uint8_t mcp2515_status();

void mcp2515_send_message(CANMessage *p_message);

uint8_t mcp2515_read_rx_status(void);
uint8_t mcp2515_read_register(uint8_t adress);
void mcp2515_read_register_cont(uint8_t adress, uint8_t* data, uint8_t length);
void mcp2515_bit_modify(uint8_t adress, uint8_t mask, uint8_t data);

#endif /* MCP2515_H_ */