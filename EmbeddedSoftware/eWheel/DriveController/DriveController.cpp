/* 
* ESC.cpp
*
* Created: 14.04.2019 15:02:13
* Author: Dominik Hellhake
*/
#include "DriveController.h"
#include "..\Peripheral\VESC\VESC.h"


#include "..\Peripheral/W25Q128/W25Q128.h"


DriveController Drive;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT DriveController::Run(uint32_t timeStamp)
{	
	RUN_RESULT result = RUN_RESULT::IDLE;
				
	if (timeStamp - this->ESC_Update_Tstmp >= 100)
	{
		
		DataFlash.ReadJEDEC();
		
		
		//ESC.Update();
		this->ESC_Update_Tstmp = timeStamp;
		result = RUN_RESULT::SUCCESS;
	}
	
	
	return result;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
DriveController::DriveController()
{
} //ESC

void DriveController::Stop()
{
	if (this->Tar_Duty > 0.0f)
		ESC.SetDuty(0.0f);
	
	this->ResetTarValues();
}


