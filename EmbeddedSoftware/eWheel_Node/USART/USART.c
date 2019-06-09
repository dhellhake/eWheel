/*
 * USART.c
 *
 * Created: 09.06.2019 13:26:42
 *  Author: Dominik Hellhake
 */
#include "USART.h"
#include "..\System.h"

void init_uart(uint32_t baudrate)
{

	uint16_t UBRR_val = (F_CPU/16)/(baudrate-1);

	UBRR0H = UBRR_val >> 8;
	UBRR0L = UBRR_val;

	UCSR0B |= (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0); // UART TX (Transmit - senden) einschalten
	UCSR0C |= (1<<USBS0) | (3<<UCSZ00); //Modus Asynchron 8N1 (8 Datenbits, No Parity, 1 Stopbit)
}

void uart_putc(uint8_t data)
{

	while(!(UCSR0A & (1<<UDRE0))); // wait until sending is possible
	UDR0 = data;
}