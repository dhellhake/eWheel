/* 
* USART.cpp
*
* Created: 01.05.2018 17:57:33
* Author: Dominik Hellhake
*/
#include "USART.h"
#include "..\..\CC41A\CC41A.h"
#include "..\..\ESC\ESC.h"

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

void SERCOM3_SendByte(uint8_t byte)
{
	while(!SERCOM3->USART.INTFLAG.bit.DRE);
	SERCOM3->USART.DATA.reg = byte;
}
void SERCOM3_Handler()
{
	if (SERCOM3->USART.INTFLAG.bit.RXC)
	{
		uint8_t rxData = SERCOM3->USART.DATA.reg;
		
		VESC.ReceiveByte(rxData);
		
		return;
	}
}