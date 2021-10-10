/*
 * Firmware.cpp
 *
 * Created: 04.06.2021 16:20:45
 * Author : dominik hellhake
 */
#include "sam.h"
#include "ATSAME70/System/System.h"
#include "ATSAME70/HSMCI/HSMCIlib.h"
#include "SDCard/SDCard.h"

#define TASKPOOL_SIZE	1

Task* taskPool[TASKPOOL_SIZE] = {
	&Disk,
};
uint16_t timeSlot[TASKPOOL_SIZE]
{
	10000,
};

typedef struct RuntimeRecord
{
	uint32_t Runtime[TASKPOOL_SIZE] { 0 };
	uint64_t ElapsedMicros	= 0;
	RecordType Type			= RecordType::Runtime;
	uint16_t Postamble		= 0xAA55;
} RuntimeRecord;

int main(void)
{
	uint64_t t_now = 0;
	uint64_t t_now_2 = 0;
	uint8_t taskIndex = 0;
	RuntimeRecord runtimeRecord;
	RUN_RESULT runResult;
		
	while (1)
	{
		t_now = System.GetElapsedMicros();
		runResult = taskPool[taskIndex]->Run(System.GetElapsedMilis());
		t_now_2 = System.GetElapsedMicros();
		
		runtimeRecord.Runtime[taskIndex] = t_now_2 - t_now;
		
		while (t_now_2 - t_now < timeSlot[taskIndex])
			t_now_2 = System.GetElapsedMicros();
		
		taskIndex++;
		if (taskIndex >= TASKPOOL_SIZE)
		{			
			#ifdef RecordRuntime
				runtimeRecord.ElapsedMicros = t_now_2;
				Disk.AddRecord((uint8_t*)&runtimeRecord);
			#endif
			taskIndex = 0;
		}
	}
}
