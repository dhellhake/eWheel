/*
 * eWheel.cpp
 *
 * Created: 30.12.2019 12:25:23
 * Author : dominik hellhake
 */
 #include "samc21.h"
 #include "LowLevel/Device/SysTick/SysTick.h"

#include "LowLevel/CC41A/CC41A.h"
#include "COMPort/COMPort.h"
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
		&ComLink,
		&BLEDevice
	};
	
	uint16_t timeSlot[TASKPOOL_SIZE] 
	{
		1000,
		1000,
		1000,
		1000,
		2000,
		4000		
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
