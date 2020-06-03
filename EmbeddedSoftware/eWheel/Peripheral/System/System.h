/* 
* System.h
*
* Created: 29.04.2020 21:30:28
* Author: dominik hellhake
*/
#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#ifdef __cplusplus
extern "C" {
#endif	
	#include "sam.h"

	#define SYSTEM_CPU_CLK		120000000UL
		
	typedef enum RUN_MODE
	{
		NORMAL		= 0x00,
		TRACE_MODE	= 0x01
	} RUN_MODE;
		
	extern volatile RUN_MODE Mode;

	// Overflow of 24bit-Systick-Counter running at 120Mhz
	extern volatile uint64_t SysTick_Overflow;

	void InitSysTick();

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
	
	inline void SoftwareReset()
	{
		RSTC->RSTC_CR = RSTC_CR_KEY_PASSWD | RSTC_CR_PROCRST | RSTC_CR_PERRST;
	}
	
	extern volatile uint32_t ElapsedMillis;
#ifdef __cplusplus
}
#endif

#endif //__SYSTEM_H__
