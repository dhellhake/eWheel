/* 
* USARTlib.cpp
*
* Created: 17.05.2020 13:42:31
* Author: dominik hellhake
*/
#include "USARTlib.h"
#include "..\..\System\System.h"
#include "..\..\VESC\VESC.h"
#include "..\..\BNO055\BNO055.h"

uint32_t usart_set_async_baudrate(Usart *usart, uint32_t baudrate, uint32_t ul_mck)
{
	uint32_t over;
	uint32_t cd_fp;
	uint32_t cd;
	uint32_t fp;

	/* Calculate the receiver sampling divide of baudrate clock. */
	if (ul_mck >= HIGH_FRQ_SAMPLE_DIV * baudrate)
		over = HIGH_FRQ_SAMPLE_DIV;
	else
		over = LOW_FRQ_SAMPLE_DIV;
	

	/* Calculate clock divider according to the fraction calculated formula. */
	cd_fp = (8 * ul_mck + (over * baudrate) / 2) / (over * baudrate);
	cd = cd_fp >> 3;
	fp = cd_fp & 0x07;
	if (cd < MIN_CD_VALUE || cd > MAX_CD_VALUE)
		return 1;

	/* Configure the OVER bit in MR register. */
	if (over == 8)
		usart->US_MR |= US_MR_OVER;

	/* Configure the baudrate generate register. */
	usart->US_BRGR = (cd << US_BRGR_CD_Pos) | (fp << US_BRGR_FP_Pos);

	return 0;
}

void InitUSART0()
{
	usart_set_async_baudrate(USART0, 230400, SYSTEM_CPU_CLK);
	
	USART0->US_MR =		US_MR_CHMODE_NORMAL |
						US_MR_NBSTOP_1_BIT |
						US_MR_PAR_NO |
						US_MR_CHRL_8_BIT |
						US_MR_USCLKS_MCK |
						US_MR_USART_MODE_NORMAL;
	
	USART0->US_CR =		US_CR_TXEN | US_CR_RXEN;
	
	USART0->US_IDR =	0xffffffff;
	USART0->US_IER =	US_IDR_RXRDY;
	
	/* Configure and enable interrupt of PIOB. */
	NVIC_EnableIRQ(USART0_IRQn);
}

void InitUSART1()
{
	usart_set_async_baudrate(USART1, 115200, SYSTEM_CPU_CLK);
	
	USART1->US_MR =		US_MR_CHMODE_NORMAL |
						US_MR_NBSTOP_1_BIT |
						US_MR_PAR_NO |
						US_MR_CHRL_8_BIT |
						US_MR_USCLKS_MCK |
						US_MR_USART_MODE_NORMAL;
	
	USART1->US_CR =		US_CR_TXEN | US_CR_RXEN;
	
	USART1->US_IDR =	0xffffffff;
	USART1->US_IER =	US_IDR_RXRDY;
	
	/* Configure and enable interrupt of PIOB. */
	NVIC_EnableIRQ(USART1_IRQn);
}

void USART1_Handler ( void )
{
	uint32_t status = USART1->US_CSR;
	
	if (status & US_CSR_RXRDY)
	{
		uint32_t d = USART1->US_RHR & US_RHR_RXCHR_Msk;
		IMU.ReceiveByte((uint8_t)d);
	}
}

void USART0_Handler ( void )
{
	uint32_t status = USART0->US_CSR;
	
	if (status & US_CSR_RXRDY)
	{
		uint32_t d = USART0->US_RHR & US_RHR_RXCHR_Msk;
		ESC.ReceiveByte((uint8_t)d);
	}
}


