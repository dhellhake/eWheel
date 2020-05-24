/* 
* UARTlib.h
*
* Created: 24.05.2020 12:42:45
* Author: domininik hellhake
*/
#ifndef __UARTLIB_H__
#define __UARTLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

	#include "sam.h"	

	void InitUART0();
	void InitUART1();	

	inline void UART_SendByte(Uart *uart, uint8_t data)
	{
		/* Check if the transmitter is ready */
		while (!(uart->UART_SR & UART_SR_TXRDY));

		/* Send character */
		uart->UART_THR = data;
	}

#ifdef __cplusplus
}
#endif

#endif //__UARTLIB_H__
