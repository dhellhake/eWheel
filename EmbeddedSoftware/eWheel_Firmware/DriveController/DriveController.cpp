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
	this->AvlRelACPD = this->GetAvlACPD();
	
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
				VESC.SetTar_Duty(0.0f);
				this->State_Dbnc = timeStamp;				
			} else if (!this->IsDroppedOver())
			{
				this->State_Dbnc = timeStamp;
				if (this->AvlRelACPD < 0.0f)
					VESC.SetTar_Duty(0.05);
				else				
					this->State = DriveState::Driving;				
			}				
		
			if (timeStamp - this->State_Dbnc >= 500)
			{
				VESC.ResetTarValues();
				this->State = DriveState::DroppedOver;
			}		
		break;
		case DriveState::Driving:		
			if (!this->IsDroppedOver())
			{				
				this->State_Dbnc = timeStamp;
								
				if (this->AvlRelACPD < -0.15f || this->AvlRelACPD > 0.15f)
				{
					if (timeStamp - LastTarValueUpdate > 100)
					{
						VESC.Tar_HandBrake = 0.0f;
						
						float tar_duty;
						
						if (this->AvlRelACPD < 0)
							tar_duty = ((this->AvlRelACPD * 0.4f) / 10);
						else
							tar_duty = ((this->AvlRelACPD * 0.4f) / 10);
						
						VESC.SetTar_Duty(VESC.Tar_Duty + tar_duty);
						
						if (VESC.Tar_Duty == 0.0f && VESC.Avl_RPM <= 200)
							VESC.Tar_HandBrake = 3.0f;
						
						
						
						LastTarValueUpdate = timeStamp;
					}
				}				
			}
			
			
			if (timeStamp - this->State_Dbnc >= 500)
			{
				VESC.ResetTarValues();
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

