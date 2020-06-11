/* 
* USARTlib.h
*
* Created: 17.05.2020 13:42:31
* Author: dominik hellhake
*/
#ifndef __USARTLIB_H__
#define __USARTLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

	#include "sam.h"
	
	/* The receiver sampling divide of baudrate clock. */
	#define HIGH_FRQ_SAMPLE_DIV           16
	#define LOW_FRQ_SAMPLE_DIV            8

	/* The CD value scope programmed in MR register. */
	#define MIN_CD_VALUE                  0x01
	#define MAX_CD_VALUE                  US_BRGR_CD_Msk

	void InitUSART0();
	void InitUSART1();
	
	inline void USART_SendByte(Usart *usart, uint8_t byte)
	{		
		while(!(usart->US_CSR & US_CSR_TXRDY));
		
		usart->US_THR = US_THR_TXCHR(byte);
	}

#ifdef __cplusplus
}
#endif

#endif //__USARTLIB_H__
