/*
 * SysTick.h
 *
 * Created: 15.03.2019 15:49:41
 *  Author: Dominik Hellhake
 */
#ifndef SYSTICK_H_
#define SYSTICK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "samc21.h"

// Overflow of 16bit-Systick-Counter running at 48Mhz
extern volatile uint64_t SysTick_Overflow;

void InitSysTick();

inline uint32_t GetElapsedMilis()
{
	uint64_t result = (SysTick_Overflow * 0xFFFFFF) + (0xFFFFFF - SysTick->VAL);
	return (uint32_t)(result / 48000);
}

inline uint64_t GetElapsedMicros()
{
	uint64_t result = (SysTick_Overflow * 0xFFFFFF) + (0xFFFFFF - SysTick->VAL);
	return (uint32_t)(result / 48);
}

#ifdef __cplusplus
}
#endif

#endif /* SYSTICK_H_ */