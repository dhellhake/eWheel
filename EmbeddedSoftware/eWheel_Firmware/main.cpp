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
		&eWheel.Gyroscope,
		&eWheel.Trace,
		&eWheel.Bluetooth
	};
	
	uint32_t t1 = 0;
    while (1) 
    {	
		t1 = GetElapsedMilis();
		
		for (uint8_t ti = 0; ti < TASKPOOL_SIZE; ti++)
			if (taskPool[ti]->IsReady(t1))
				taskPool[ti]->Run(t1);
		
		// If last task is complete, reset computation
		if (taskPool[0]->Status == TASK_STATUS::COMPLETE)
			for (uint8_t ti = 0; ti < TASKPOOL_SIZE; ti++)
				taskPool[ti]->Status =TASK_STATUS::WAIT;
    }
}
