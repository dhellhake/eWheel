/*
 * eWheel_Firmware
 *
 * Created: 08.07.2018 08:52:44
 * Author : Dominik Hellhake
 */
#include "samc21.h"
#include "LowLevel/Device/SysTick/SysTick.h"

#include "AT45DB/AT45DB.h"
#include "CC41A/CC41A.h"
#include "Chassis/Chassis.h"
#include "DriveController/DriveController.h"
#include "ESC/ESC.h"
#include "LSM9D/LSM9D.h"

#define TASKPOOL_SIZE	6

void HardFault_Handler(void)
{
}

int main(void)
{			
	Executable* taskPool[TASKPOOL_SIZE] = {
		&Drive,
		&Board,
		&VESC,
		&Gyro,
		&Trace,
		&Bluetooth
	};
	
	uint32_t t1 = 0;
    while (1) 
    {	
		t1 = GetElapsedMilis();
		
		for (uint8_t ti = 0; ti < TASKPOOL_SIZE; ti++)
			if (taskPool[ti]->IsReady(t1))
				taskPool[ti]->Run(t1);
		
		// If last task is complete, reset computation
		if (taskPool[0]->TaskStatus == TASK_STATUS::COMPLETE)
			for (uint8_t ti = 0; ti < TASKPOOL_SIZE; ti++)
				taskPool[ti]->ResetTask();
    }
}
