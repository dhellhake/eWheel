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
				this->DropOver_Dbnc = timeStamp;
		
			if (timeStamp - this->DropOver_Dbnc >= 700)
			{
				this->State = DriveState::Starting;					
			}		
		break;
		case DriveState::Starting:
			if (this->IsStarting())
			{
				VESC.SetHandBrake(6.0f);
				this->DropOver_Dbnc = timeStamp;
				this->State_Dbnc = timeStamp;	
			} else if (!this->IsDroppedOver())
			{
				this->DropOver_Dbnc = timeStamp;
				
				this->State = DriveState::Balancing;
				this->State_Dbnc = timeStamp;				
			}				
		
			if (timeStamp - this->DropOver_Dbnc >= 200)
			{
				VESC.ResetTarValues();
				this->State = DriveState::DroppedOver;
			}		
		break;
		case DriveState::Balancing:
			if (!this->IsDroppedOver())
			{
				this->DropOver_Dbnc = timeStamp;
				
				VESC.SetHandBrake(6.0f);
				
				if (timeStamp - this->State_Dbnc >= 100)
				{
					this->State = DriveState::Driving;
					this->State_Dbnc = timeStamp;
				}
			}
		
			if (timeStamp - this->DropOver_Dbnc >= 500)
			{
				VESC.ResetTarValues();
				this->State = DriveState::DroppedOver;
			}
		
		break;
		case DriveState::Driving:		
			if (!this->IsDroppedOver())
			{				
				this->DropOver_Dbnc = timeStamp;
								
				if (this->AvlRelACPD < -0.15f || this->AvlRelACPD > 0.15f)
				{
					if (timeStamp - LastTarValueUpdate > 100)
					{						
						float tar_duty;
						
						if (this->AvlRelACPD < 0)
							tar_duty = ((this->AvlRelACPD * 0.2f) / 10);
						else
							tar_duty = ((this->AvlRelACPD * 0.2f) / 10);
						
						VESC.SetTar_Duty(VESC.Tar_Duty + tar_duty);
						
						if (VESC.Tar_Duty == 0.0f && VESC.Avl_RPM <= 200)
							VESC.SetHandBrake(6.0f);
						
						LastTarValueUpdate = timeStamp;
					}
				}				
			}
			
			
			if (timeStamp - this->DropOver_Dbnc >= 300)
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

