/*
 * eWheel_Firmware
 *
 * Created: 08.07.2018 08:52:44
 * Author : Dominik Hellhake
 */
#include "samc21.h"
#include "System/System.h"

#define HIGHPRIORITY_POOL_SIZE	3
#define LOWPRIORITY_POOL_SIZE	3

int main(void)
{	
	Executable* highPriorityPool[HIGHPRIORITY_POOL_SIZE] = {
		&eWheel.Orientation,
		&eWheel.AnalogSensor,
		&eWheel.InfraRedSensor	 
	};	
	
	Executable* lowPriorityPool[LOWPRIORITY_POOL_SIZE] = {
		&eWheel.TraceLink,
		&eWheel.DebugLink,
		&eWheel.OLED
	};
	uint8_t lowPriorityIndex = 0;
	
	uint64_t t1 = GetElapsedMilis();
	bool slotUsed = false;
    while (1) 
    {		
		if ((t1 + 10) <= GetElapsedMilis())
		{
			t1+= 10;
			slotUsed = false;
			
			for (uint8_t ti = 0; ti < HIGHPRIORITY_POOL_SIZE; ti++)
			{
				if (highPriorityPool[ti]->CanExecute())
				{
					slotUsed = true;
					if (highPriorityPool[ti]->Run() == RUN_RESULT::SUCCESS)
					{					
						highPriorityPool[ti]->Propagate();
						break;
					}					
				}
			}
			
			if (!slotUsed)
			{
				for (uint8_t ti = lowPriorityIndex; ti < LOWPRIORITY_POOL_SIZE; ti++)
				{
					if (lowPriorityPool[ti]->CanExecute())
					{
						slotUsed = true;
						if (lowPriorityPool[ti]->Run() == RUN_RESULT::SUCCESS)
						{
							lowPriorityPool[ti]->Propagate();
							break;
						}
					}
				}
				
				lowPriorityIndex++;
				if (lowPriorityIndex >= LOWPRIORITY_POOL_SIZE)
					lowPriorityIndex = 0;
			}			
		}
    }
}
