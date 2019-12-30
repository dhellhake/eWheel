/* 
* USART.h
*
* Created: 01.05.2018 17:57:33
* Author: Dominik Hellhake
*/
#ifndef __USART_H__
#define __USART_H__

#include "samc21.h"

void SERCOM5_SendByte(uint8_t byte);
void SERCOM1_SendByte(uint8_t byte);

#endif //__USART_H__
