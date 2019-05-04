/* 
* DriveController.cpp
*
* Created: 04.05.2019 09:39:37
* Author: Dominik Hellhake
*/
#include "DriveController.h"

DriveController Drive;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT DriveController::Run(uint32_t timeStamp)
{
	switch(this->State)
	{
		case DriveState::DroppedOver:
			if (!this->IsStarting())
				this->State_Dbnc = timeStamp;
		
			if (timeStamp - this->State_Dbnc >= 1000)
			{
				this->State = DriveState::Starting;					
			}		
		break;
		case DriveState::Starting:
			if (this->IsStarting())
			{
				this->State_Dbnc = timeStamp;				
			} else if (!this->IsDroppedOver())
			{
				this->State = DriveState::Driving;					
			}				
		
			if (timeStamp - this->State_Dbnc >= 500)
			{
				this->State = DriveState::DroppedOver;
			}		
		break;
		case DriveState::Driving:
		
			if (!this->IsDroppedOver())
				this->State_Dbnc = timeStamp;
			
			if (timeStamp - this->State_Dbnc >= 500)
			{
				this->State = DriveState::DroppedOver;
			}
		break;
	}	
	
	Board.ResetTask();
		
	this->TaskStatus = TASK_STATUS::COMPLETE;
	return RUN_RESULT::SUCCESS;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
DriveController::DriveController()
{
	this->State = DriveState::DroppedOver;
} //DriveController

