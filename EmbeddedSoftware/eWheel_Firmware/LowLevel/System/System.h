/* 
* System.h
*
* Created: 08.05.2018 18:22:30
* Author: dominik hellhake
*/


#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <string.h>
#include "samc21.h"
#include "..\..\Executable.h"

#define COMPILER_ALIGNED(a) __attribute__((__aligned__(a)))

#define TASKPOOL_SIZE 5


class System
{
//variables
public:
	volatile static uint64_t SysTick_Overflow;
	
	volatile static bool DMAC_TX_Complete[1];
	
	static Executable* TaskPool[TASKPOOL_SIZE];
//functions
public:
	static void Init();
	
	static void StartDMATransfer(DmacDescriptor *descriptor, uint8_t dmaSlot);
	
	static void SetPinPeripheralFunction(uint32_t pinmux);
	
	static uint32_t GetGCLK_Hz(uint32_t gclk_id);
	
	static uint32_t GetElapsedMilis();
	
	static inline void SetLED(bool state)
	{
		if (state)
			PORT->Group[0].OUTSET.reg = PORT_PA28;
		else
			PORT->Group[0].OUTSET.reg = PORT_PA28;
	}
	
	static inline float DIVAS_SQRT(float radicand)
	{
		/* Write the radicand to DIVIDEND register. */
		DIVAS->SQRNUM.reg = radicand;

		while(DIVAS->STATUS.bit.BUSY){}

		return (float)DIVAS->RESULT.reg;
	}
	
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
		
private:
	System();
		
	static void InitGCLK();
	
	static void InitSysTick();
	
	static void InitLED();
	
	static void InitEIC();
	
	static void InitDMAC();
	
	~System();
	System( const System &c );
	System& operator=( const System &c );

}; //System

#endif //__SYSTEM_H__
