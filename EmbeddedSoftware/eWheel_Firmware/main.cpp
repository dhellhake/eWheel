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
	
	uint32_t t0 = 0;
	uint32_t t1 = 0;
	
	uint8_t taskIndex = 0;
    while (1) 
    {	
		t1 = GetElapsedMilis();
		if (t1 - t0 >= 10)
		{
			taskPool[taskIndex]->Run(t1);

			taskIndex++;
			if (taskIndex >= TASKPOOL_SIZE)
				taskIndex = 0;
			
			t0 = t1;
		}		
    }
}
