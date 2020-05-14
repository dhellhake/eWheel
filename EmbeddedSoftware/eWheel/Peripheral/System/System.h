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

	void InitSysTick();
	
	extern volatile uint32_t ElapsedMillis;
#ifdef __cplusplus
}
#endif

#endif //__SYSTEM_H__
