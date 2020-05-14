/* 
* WDTlib.h
*
* Created: 29.04.2020 21:24:07
* Author: dominik hellhake
*/
#ifndef __WDTLIB_H__
#define __WDTLIB_H__

#ifdef __cplusplus
extern "C" {
#endif	
	#include "sam.h"

	inline void DisableWatchdog() 
	{
		/* Disable the watchdog */
		WDT->WDT_MR = WDT_MR_WDDIS;
	}

#ifdef __cplusplus
}
#endif

#endif //__WDTLIB_H__
