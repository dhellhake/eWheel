/* 
* Chassis.cpp
*
* Created: 28.04.2019 20:30:49
* Author: Dominik Hellhake
*/
#include "Chassis.h"
#include "..\LSM9D\LSM9D.h"
#include "..\LowLevel\Device\SysTick\SysTick.h"

Chassis Board;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT Chassis::Run(uint32_t timeStamp)
{
	if (Gyro.TaskStatus != TASK_STATUS::COMPLETE)
		return RUN_RESULT::IDLE;
	
	this->Chassis_Pitch = Gyro.Pitch - this->Chassis_Pitch;	
	this->Chassis_Roll = Gyro.Roll - this->Road_Roll;	
	
	switch (this->State)
	{
		case ChassisState::Starting:
			if (!this->IsStartPosition())
				this->State = ChassisState::Normal;		
		break;
		case ChassisState::Normal:			
			if (this->IsRolledOver() || this->IsPitchedOver())
				this->DropOver_Dbnc += timeStamp - this->LAST_RUNNED;
			else
				this->DropOver_Dbnc = 0;
						
			if (this->DropOver_Dbnc > 200)
				this->State = ChassisState::DroppedOver;
		break;
		case ChassisState::DroppedOver:
			if (this->IsStartPosition())
				this->State = ChassisState::Starting;
		break;
	}
	
	if (this->TraceEnabled)
		Trace();
	
	this->TaskStatus = TASK_STATUS::COMPLETE;
	return RUN_RESULT::SUCCESS;
}

void Chassis::Trace()
{
	uint32_t* iPtr = (uint32_t*)this->Page._data + ((this->PageBlock_Ind * CHASSIS_FLASH_PAGE_BLOCK_SIZE) / 4);
	*iPtr = GetElapsedMilis();
	
	float* fPtr = (float*)this->Page._data + ((this->PageBlock_Ind * CHASSIS_FLASH_PAGE_BLOCK_SIZE) / 4);
	
	fPtr += 1;
	*fPtr = Gyro.Pitch;
	
	fPtr += 1;
	*fPtr = Gyro.Roll;
	
	this->PageBlock_Ind++;
	if (this->PageBlock_Ind == CHASSIS_FLASH_PAGE_BLOCK_CNT)
	{
		this->Page._Sequence = this->PageSequence_Ind;
		Flash.MemPage_Write(&this->Page, true);
		this->PageSequence_Ind++;
		this->PageBlock_Ind = 0;
	}
}


/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
Chassis::Chassis()
{
	this->Page._type = BoardTrace;
} //Chassis

void Chassis::SetLED(bool state)
{
	if (state)
		PORT->Group[0].OUTSET.reg = PORT_PA28;
	else
		PORT->Group[0].OUTCLR.reg = PORT_PA28;
}