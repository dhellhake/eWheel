/*
 * eWheel.cpp
 *
 * Created: 13.05.2020 21:45:06
 * Author : dominik hellhake
 */
#include <string.h>
#include "sam.h"
#include "Peripheral/System/System.h"
#include "ESC/DriveController.h"

#define TASKPOOL_SIZE	1

int main(void)
{				
	Task* taskPool[TASKPOOL_SIZE] = {
		&Drive
	};
	
	uint16_t timeSlot[TASKPOOL_SIZE]
	{
		1000,
	};
	
	uint64_t t_now = 0;
	uint64_t t_now_2 = 0;
	uint8_t taskIndex = 0;
	uint32_t runtime[TASKPOOL_SIZE] { 0 };
	while (1)
	{
		t_now = GetElapsedMicros();
		if (taskPool[taskIndex]->Run(GetElapsedMilis()) == RUN_RESULT::SUCCESS)
		taskPool[taskIndex]->LAST_RUNNED = t_now;
		t_now_2 = GetElapsedMicros();
		
		if (runtime[taskIndex] < t_now_2 - t_now)
		runtime[taskIndex] = t_now_2 - t_now;
		
		while (t_now_2 - t_now < timeSlot[taskIndex])
			t_now_2 = GetElapsedMicros();
		
		taskIndex++;
		if (taskIndex >= TASKPOOL_SIZE)
			taskIndex = 0;
	}
}
