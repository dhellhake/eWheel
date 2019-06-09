/*
 * USART.h
 *
 * Created: 09.06.2019 13:26:55
 *  Author: Dominik Hellhake
 */ 
#ifndef USART_H_
#define USART_H_

#include <avr/io.h>

void init_uart(uint32_t baudrate);

void uart_putc(uint8_t data);

#endif /* USART_H_ */