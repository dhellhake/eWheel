/*
 * eWheel_Firmware
 *
 * Created: 08.07.2018 08:52:44
 * Author : Dominik Hellhake
 */
#include "samc21.h"
#include "System/System.h"


int main(void)
{		
	uint64_t t1 = GetElapsedMilis();
    while (1) 
    {		
		if ((t1 + 10) <= GetElapsedMilis())
		{
			t1+= 10;
			
			for (uint8_t ti = 0; ti < TASKPOOL_SIZE; ti++)
			{
				if (eWheel.TaskPool[ti]->CanExecute())
				{
					if (eWheel.TaskPool[ti]->Run() == RUN_RESULT::SUCCESS)
					{					
						eWheel.TaskPool[ti]->Propagate();
						break;
					}					
				}
			}
		}
    }
}
