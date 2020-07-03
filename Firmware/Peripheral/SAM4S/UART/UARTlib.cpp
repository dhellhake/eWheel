/* 
* UARTlib.cpp
*
* Created: 24.05.2020 12:42:45
* Author: dominik hellhake
*/
#include "UARTlib.h"
#include "..\..\CC41A\CC41A.h"
#include "..\..\CP2102\CP2102.h"

void InitUART0()
{
	/* Reset and disable receiver & transmitter */
	UART0->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;
	/* Set baudrate 120Mhz/16/33 = 227272,7273 */
	UART0->UART_BRGR = 33;
	/* Configure mode */
	UART0->UART_MR = UART_MR_CHMODE_NORMAL | UART_MR_PAR_NO;
	/* Disable PDC channel */
	UART0->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
	
	/* Enable UART interrupts. */
	UART0->UART_IDR = 0xFFFFFFFF;
	UART0->UART_IER = UART_IER_RXRDY;
	
	/* Enable receiver and transmitter */
	UART0->UART_CR = UART_CR_RXEN | UART_CR_TXEN;
	
	/* Configure and enable interrupt of PIOB. */
	NVIC_EnableIRQ(UART0_IRQn);
}


void UART0_Handler()
{
	uint32_t status = UART0->UART_SR;
	
	if (status & UART_SR_RXBUFF)
	{
		uint8_t data = UART0->UART_RHR;
		BLEPort.ReceiveByte(data);
	}
}

void InitUART1()
{	
	/* Reset and disable receiver & transmitter */
	UART1->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;
	/* Set baudrate 120Mhz/16/8 = 937500 */
	UART1->UART_BRGR = 8;
	/* Configure mode */
	UART1->UART_MR = UART_MR_CHMODE_NORMAL | UART_MR_PAR_NO;
	/* Disable PDC channel */
	UART1->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;
	
	/* Enable UART interrupts. */
	UART1->UART_IDR = 0xFFFFFFFF;
	UART1->UART_IER = UART_IER_RXRDY;
	
	/* Enable receiver and transmitter */
	UART1->UART_CR = UART_CR_RXEN | UART_CR_TXEN;
	
	/* Configure and enable interrupt of PIOB. */
	NVIC_EnableIRQ(UART1_IRQn);
}


void UART1_Handler()
{
	uint32_t status = UART1->UART_SR;
	
	if (status & UART_SR_RXBUFF)
	{
		uint8_t data = UART1->UART_RHR;
		DiagPort.ReceiveByte(data);
	}
}