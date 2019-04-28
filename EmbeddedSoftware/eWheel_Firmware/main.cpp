/*
 * eWheel_Firmware
 *
 * Created: 08.07.2018 08:52:44
 * Author : Dominik Hellhake
 */
#include "samc21.h"
#include "LowLevel/Device/SysTick/SysTick.h"
#include "System/System.h"

#define TASKPOOL_SIZE	4

int main(void)
{	
	Executable* taskPool[TASKPOOL_SIZE] = {
		&eWheel.vESC,
		&eWheel.Orientation,
		&eWheel.TraceLink,
		&eWheel.DebugLink
	};
	
	uint32_t t1 = GetElapsedMilis();
    while (1) 
    {		
		if (t1 <= GetElapsedMilis())
		{			
			for (uint8_t ti = 0; ti < TASKPOOL_SIZE; ti++)
				if (taskPool[ti]->Run(t1) == RUN_RESULT::SUCCESS)
				break;		
			t1+= 10;			
		}
    }
}
