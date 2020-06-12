/* 
* System.h
*
* Created: 29.04.2020 21:30:28
* Author: dominik hellhake
*/
#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "sam.h"

#define SYSTEM_CPU_CLK		120000000UL

class Sys
{
	private:		
		volatile uint32_t ElapsedMillis;
	
	public:
		// Overflow of 24bit-Systick-Counter running at 120Mhz
		volatile uint64_t SysTick_Overflow;
	
		Sys();
	
		inline uint32_t GetElapsedMilis()
		{
			uint64_t result = (SysTick_Overflow * 0xFFFFFF) + (0xFFFFFF - SysTick->VAL);
			return (uint32_t)(result / 120000);
		}

		inline uint64_t GetElapsedMicros()
		{
			uint64_t result = (SysTick_Overflow * 0xFFFFFF) + (0xFFFFFF - SysTick->VAL);
			return (uint32_t)(result / 120);
		}	
	
		inline void SoftReset()
		{
			RSTC->RSTC_CR = RSTC_CR_KEY_PASSWD | RSTC_CR_PROCRST | RSTC_CR_PERRST;
		}	
}; //System

extern Sys System;


#endif //__SYSTEM_H__
