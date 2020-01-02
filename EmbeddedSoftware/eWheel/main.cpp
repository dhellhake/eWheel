/*
 * eWheel.cpp
 *
 * Created: 30.12.2019 12:25:23
 * Author : dominik hellhake
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
	
	FlashPage page;
	
	uint32_t t_now = 0;
	uint32_t t_now_2 = sizeof(page);
	
	uint8_t taskIndex = 0;
	while (1)
	{
		t_now = GetElapsedMilis();
		
		if (taskPool[taskIndex]->Run(t_now) == RUN_RESULT::SUCCESS)
			taskPool[taskIndex]->LAST_RUNNED = t_now;

		t_now_2 = GetElapsedMilis();		
		while (t_now_2 - t_now < 5)
			t_now_2 = GetElapsedMilis();

		taskIndex++;
		if (taskIndex >= TASKPOOL_SIZE)
			taskIndex = 0;
		
		if (Drive.TaskStatus == TASK_STATUS::COMPLETE)
			for (uint8_t ti = 0; ti < TASKPOOL_SIZE; ti++)
				taskPool[ti]->Reset();
	}
}
