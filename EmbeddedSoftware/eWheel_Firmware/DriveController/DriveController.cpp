/* 
* DriveController.cpp
*
* Created: 04.05.2019 09:39:37
* Author: Dominik Hellhake
*/
#include <math.h>
#include "DriveController.h"

DriveController Drive;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT DriveController::Run(uint32_t timeStamp)
{
	if (Board.TaskStatus != TASK_STATUS::COMPLETE)
		return RUN_RESULT::IDLE;
			
	this->AvlRelACPD = this->GetAvlACPD();
	
	
	if (Board.State == ChassisState::Starting && VESC.Avl_RPM < 200)
	{
		this->State = DriveState::Starting;
		this->State_Dbnc = 0;
	} else if (Board.State == ChassisState::DroppedOver)
	{
		if (this->State != DriveState::Stopped)
			VESC.Stop();
		
		this->State = DriveState::Stopped;
		this->State_Dbnc = 0;
	}
		
	switch(this->State)
	{
		case DriveState::Stopped:
			VESC.SetBrake(4.0f);
			if (Board.State == ChassisState::Starting)
			{
				this->State_Dbnc += timeStamp - this->LAST_RUNNED;
				
				if (this->State_Dbnc > 700)
				{
					this->State = DriveState::Starting;
					this->State_Dbnc = 0;
				}				
			} else
				this->State_Dbnc = 0;
		break;
		case DriveState::Starting:			
			VESC.ResetTarValues();
			
			if (Board.State == ChassisState::Normal)
				this->State_Dbnc += timeStamp - this->LAST_RUNNED;
			else
				this->State_Dbnc = 0;
				
			if (this->State_Dbnc > 500)
			{
				this->State_Dbnc = 0;
				this->State = DriveState::Balancing;
			}
		break;
		case DriveState::Balancing:
		{	
			VESC.ResetTarValues();
						
			if (Board.Chassis_Pitch < 16.0f && Board.Chassis_Pitch > -16.0f)
			{
				float error = (0 - Board.Chassis_Pitch);
				
				float deriv = error - this->LastCtrlError;
				this->LastCtrlError = error;
				
				float diff_duty = (error * 0.05f) + (deriv * 0.1f);
												
				if (diff_duty > 0.2f)
					diff_duty = 0.2f;
				else if (diff_duty < -0.2f)
					diff_duty = -0.2f;
				
				
				this->pitch_trc[this->Trc_Ind] = Board.Chassis_Pitch;
				this->diff_duties[this->Trc_Ind] = deriv;
				this->Trc_Ind++;
				
				if (this->Trc_Ind >= 2000)
					this->Trc_Ind = 0;
					
				
				
												
				VESC.SetTar_Duty(diff_duty);
			}
			
			this->State_Dbnc += timeStamp - this->LAST_RUNNED;			
			if (this->State_Dbnc >= 3000)
			{
				//this->State = DriveState::Driving;
				this->State_Dbnc = 0;
			}	
		}
		break;
		case DriveState::Driving:		
			if (Board.State == ChassisState::Starting && VESC.Avl_RPM <= 200)
				this->State_Dbnc += timeStamp - this->LAST_RUNNED;
			
			if (this->State_Dbnc >= 1000)
			{
				this->State = DriveState::Starting;
				this->State_Dbnc = 0;
			}
		
			if (this->AvlRelACPD < -0.15f || this->AvlRelACPD > 0.15f)
			{
				float tar_duty;
				
				if (this->AvlRelACPD < 0)
				tar_duty = ((this->AvlRelACPD * 0.3f) / 10);
				else
				tar_duty = ((this->AvlRelACPD * 0.2f) / 10);
				
				VESC.SetTar_Duty(VESC.Tar_Duty + tar_duty);
				
				if (VESC.Tar_Duty == 0.0f && VESC.Avl_RPM <= 200)
				VESC.SetHandBrake(6.0f);
			}				
		break;
		}
	
	this->TaskStatus = TASK_STATUS::COMPLETE;
	return RUN_RESULT::SUCCESS;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
DriveController::DriveController()
{
	this->State = DriveState::Stopped;
} //DriveController

