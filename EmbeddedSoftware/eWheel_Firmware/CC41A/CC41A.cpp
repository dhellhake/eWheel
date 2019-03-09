/* 
* CC41A.cpp
*
* Created: 09.03.2019 18:06:43
* Author: dominik hellhake
*/
#include "CC41A.h"

void SERCOM1_Handler()
{
	if (SERCOM1->USART.INTFLAG.bit.RXC)
	{
		uint8_t rxData = SERCOM1->USART.DATA.reg;
		
		return;
	}
}

CC41A::CC41A()
{
	USART::InitSERCOM1();
} //CC41A
