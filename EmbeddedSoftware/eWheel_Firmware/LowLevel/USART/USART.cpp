/* 
* USART.cpp
*
* Created: 01.05.2018 17:57:33
* Author: Dominik Hellhake
*/
#include "USART.h"
#include "..\..\CC41A\CC41A.h"

void SERCOM5_SendByte(uint8_t byte)
{
	while(!SERCOM5->USART.INTFLAG.bit.DRE);
	SERCOM5->USART.DATA.reg = byte;
}

void SERCOM1_SendByte(uint8_t byte)
{
	while(!SERCOM1->USART.INTFLAG.bit.DRE);
	SERCOM1->USART.DATA.reg = byte;
}

void SERCOM1_Handler()
{
	if (SERCOM1->USART.INTFLAG.bit.RXC)
	{
		uint8_t rxData = SERCOM1->USART.DATA.reg;
		
		Bluetooth.ReceiveByte(rxData);
		
		return;
	}
}