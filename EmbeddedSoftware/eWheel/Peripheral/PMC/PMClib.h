/* 
* PMClib.h
*
* Created: 29.04.2020 21:04:23
* Author: dominik hellhake
*/
#ifndef __PMCLIB_H__
#define __PMCLIB_H__

#ifdef __cplusplus
extern "C" {
#endif
	#include "sam.h"

	void InitPMC();

	inline void EnablePeripheral(uint32_t pid)
	{	
		if (pid < 32)	
			PMC->PMC_PCER0 = (1 << pid);
		else
			PMC->PMC_PCER1 = (1 << (pid - 32));
	}

#ifdef __cplusplus
}
#endif

#endif //__PMCLIB_H__
