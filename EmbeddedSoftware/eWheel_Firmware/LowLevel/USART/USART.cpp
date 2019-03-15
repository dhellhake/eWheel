/* 
* USART.cpp
*
* Created: 01.05.2018 17:57:33
* Author: Dominik Hellhake
*/
#include "USART.h"

void USART::SendByte(uint8_t byte)
{
	while(!SERCOM5->USART.INTFLAG.bit.DRE);
	SERCOM5->USART.DATA.reg = byte;
}

void USART::SERCOM1_SendByte(uint8_t byte)
{
	while(!SERCOM1->USART.INTFLAG.bit.DRE);
	SERCOM1->USART.DATA.reg = byte;
}
