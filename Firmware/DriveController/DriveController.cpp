/* 
* ESC.cpp
*
* Created: 14.04.2019 15:02:13
* Author: Dominik Hellhake
*/
#include "DriveController.h"
#include "..\Peripheral\VESC\VESC.h"

DriveController Drive;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT DriveController::Run(uint32_t timeStamp)
{	
	RUN_RESULT result = RUN_RESULT::IDLE;
	
	return result;
}

RUN_RESULT DriveController::Setup(uint32_t timeStamp)
{
	return RUN_RESULT::SUCCESS;
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


