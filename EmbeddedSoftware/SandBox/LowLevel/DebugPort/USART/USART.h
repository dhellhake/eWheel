/* 
* USART.h
*
* Created: 01.05.2018 17:57:33
* Author: dominik hellhake
*/


#ifndef __USART_H__
#define __USART_H__

#include "sam.h"
#include "..\..\system.h"

class USART
{
//variables
public:
protected:
private:

//functions
public:
	void SendByte(uint8_t byte);
protected:
	USART();
	~USART();
private:
	USART( const USART &c );
	USART& operator=( const USART &c );

	/*
	* internal Calculate 64 bit division, ref can be found in
	* http://en.wikipedia.org/wiki/Division_algorithm#Long_division
	*/
	static inline uint64_t long_division(uint64_t n, uint64_t d)
	{
		int32_t i;
		uint64_t q = 0, r = 0, bit_shift;
		
		for (i = 63; i >= 0; i--)
		{
			bit_shift = (uint64_t)1 << i;
			r = r << 1;
			if (n & bit_shift)
			{
				r |= 0x01;
			}
			
			if (r >= d)
			{
				r = r - d;
				q |= bit_shift;
			}
		}
		return q;
	}

	uint16_t inline calculate_baud_value(const uint32_t baudrate, const uint32_t peripheral_clock, uint8_t sample_num)
	{
		/* Temporary variables */
		uint64_t ratio = 0;
		uint64_t scale = 0;
		uint64_t baud_calculated = 0;
		uint64_t temp1;
		/* Calculate the BAUD value */
		temp1 = ((sample_num * (uint64_t)baudrate) << 32);
		ratio = long_division(temp1, peripheral_clock);
		scale = ((uint64_t)1 << 32) - ratio;
		baud_calculated = (65536 * scale) >> 32;
		return baud_calculated;
	}


}; //USART

#endif //__USART_H__
