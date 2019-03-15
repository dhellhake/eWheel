/* 
* USART.h
*
* Created: 01.05.2018 17:57:33
* Author: Dominik Hellhake
*/
#ifndef __USART_H__
#define __USART_H__

#include "samc21.h"

class USART
{
	public:
		static void SendByte(uint8_t byte);
		static void SERCOM1_SendByte(uint8_t byte);
	protected:	
	private:
		USART();
		~USART();
		USART( const USART &c );
		USART& operator=( const USART &c );
}; //USART

#endif //__USART_H__
