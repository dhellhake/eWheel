/*
 * MCP2515.h
 *
 * Created: 09.06.2019 14:32:22
 *  Author: Dominik Hellhake
 */
#ifndef MCP2515_H_
#define MCP2515_H_

#include <avr/io.h>

void mcp2515_init();

uint8_t mcp2515_read_register(uint8_t adress);

#endif /* MCP2515_H_ */