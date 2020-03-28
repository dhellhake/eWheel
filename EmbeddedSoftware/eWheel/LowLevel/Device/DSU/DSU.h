/*
 * DSU.h
 *
 * Created: 28.03.2020 19:06:30
 *  Author: Dominik Hellhake
 */ 
#ifndef DSU_H_
#define DSU_H_

#ifdef __cplusplus
extern "C" {
#endif

	#include "samc21.h"
		
	#define SYSTEM_PERIPHERAL_ID(peripheral)    ID_##peripheral

	inline void InitDSU()
	{		
		MCLK->APBBMASK.bit.DSU_ = 1;
	}

	uint32_t CalculateCRC32(const uint32_t *addr, const uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* DSU_H_ */