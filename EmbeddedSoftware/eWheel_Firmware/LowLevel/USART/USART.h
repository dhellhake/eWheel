/* 
* USART.h
*
* Created: 01.05.2018 17:57:33
* Author: dominik hellhake
*/


#ifndef __USART_H__
#define __USART_H__

#include "sam.h"
#include "..\System\System.h"

class USART
{
	public:
		static void SendByte(uint8_t byte);
		static void SERCOM1_SendByte(uint8_t byte);
		static void InitSERCOM1();
		static void InitSERCOM5();
	protected:	
	private:
		~USART();
		USART( const USART &c );
		USART& operator=( const USART &c );
	
		static uint16_t calculate_baud_value(const uint32_t baudrate, const uint32_t peripheral_clock, uint8_t sample_num)
		{
			/* Temporary variables */
			uint64_t ratio = 0;
			uint64_t scale = 0;
			uint64_t baud_calculated = 0;
			uint64_t temp1;
			/* Calculate the BAUD value */
			temp1 = ((sample_num * (uint64_t)baudrate) << 32);
			ratio = System::long_division(temp1, peripheral_clock);
			scale = ((uint64_t)1 << 32) - ratio;
			baud_calculated = (65536 * scale) >> 32;
			return baud_calculated;
		}
}; //USART

#endif //__USART_H__
