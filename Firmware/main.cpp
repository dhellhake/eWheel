/*
 * eWheel.cpp
 *
 * Created: 13.05.2020 21:45:06
 * Author : dominik hellhake
 */
#include <string.h>
#include "sam.h"
#include "Peripheral/System/System.h"
#include "DriveController/DriveController.h"
#include "Board/Board.h"
#include "DiagLink/DiagLink.h"

#define TASKPOOL_SIZE	3

int main(void)
{		
	Task* taskPool[TASKPOOL_SIZE] = {
		&Drive,
		&Chassis,
		&Diagnostic
	};
	
	uint16_t timeSlot[TASKPOOL_SIZE]
	{
		1000,
		1000,
		8000
	};
	
	uint64_t t_now = 0;
	uint64_t t_now_2 = 0;
	uint8_t taskIndex = 0;
	uint32_t runtime[TASKPOOL_SIZE] { 0 };
	while (1)
	{		
		t_now = System.GetElapsedMicros();
		if (taskPool[taskIndex]->Run(System.GetElapsedMilis()) == RUN_RESULT::SUCCESS)
		taskPool[taskIndex]->LAST_RUNNED = t_now;
		t_now_2 = System.GetElapsedMicros();
		
		if (runtime[taskIndex] < t_now_2 - t_now)
			runtime[taskIndex] = t_now_2 - t_now;
		
		while (t_now_2 - t_now < timeSlot[taskIndex])
			t_now_2 = System.GetElapsedMicros();
		
		taskIndex++;
		if (taskIndex >= TASKPOOL_SIZE)
			taskIndex = 0;
	}
}


extern "C" {
	int diag(void)
	{
		Diagnostic.SetDiagnosticMode();
		
		while (1)
			((Task*)&Diagnostic)->Run(System.GetElapsedMilis());
	}
};