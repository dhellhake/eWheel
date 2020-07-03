/*
 * eWheel.cpp
 *
 * Created: 13.05.2020 21:45:06
 * Author : dominik hellhake
 */
#include <string.h>
#include "sam.h"
#include "Peripheral/System/System.h"
#include "Peripheral/ADS1118/ADS1118.h"
#include "Peripheral/BNO055/BNO055.h"
#include "Peripheral/VESC/VESC.h"
#include "Peripheral/W25Q128/W25Q128.h"

#include "DriveController/DriveController.h"
#include "Board/Board.h"
#include "DiagLink/DiagLink.h"
#include "BLELink/BLELink.h"

#define TASKPOOL_SIZE	8

Task* taskPool[TASKPOOL_SIZE] = {
	&ESC,
	&ADS,
	&IMU,
	&DataFlash,
	&Drive,
	&Chassis,
	&Diagnostic,
	&BLE
};
uint16_t timeSlot[TASKPOOL_SIZE]
{
	200,
	100,
	200,
	1000,
	1000,
	1000,
	1000,
	5500
};

void Setup();


int main(void)
{			
	uint64_t t_now = 0;
	uint64_t t_now_2 = 0;
	uint8_t taskIndex = 0;
	uint32_t runtime[TASKPOOL_SIZE] { 0 };
	RUN_RESULT runResult;
	
	
	Setup();
	
	while (1)
	{		
		t_now = System.GetElapsedMicros();		
		runResult = taskPool[taskIndex]->Run(System.GetElapsedMilis());		
		t_now_2 = System.GetElapsedMicros();
		
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
		Setup();
		Diagnostic.SetDiagnosticMode();
		
		while (1)
			((Task*)&Diagnostic)->Run(System.GetElapsedMilis());
	}
};

/************************************************************************/
/* Setup of each task after a short delay at startup                    */
/************************************************************************/
void Setup()
{
	uint64_t t_now = 0;
	while (t_now <= 500)
	t_now = System.GetElapsedMilis();
	
	for (uint8_t idx = 0; idx < TASKPOOL_SIZE; idx++)
	taskPool[idx]->Setup(System.GetElapsedMilis());
}