/* 
* MotorDriver.cpp
*
* Created: 12.05.2018 13:25:12
* Author: Dominik Hellhake
*/
#include "MotorController.h"
#include "..\System\System.h"

MotorController::MotorController()
{
	this->SetDuty(this->PhaseDuty);	
	
}


void MotorController::SetState(CONTROLLER_STATE state)
{
	this->ControllerState = state;
	
	switch(state)
	{
		case CONTROLLER_STATE::IDLE:
			TCC0->PATT.reg = BLDC_PATTERN_IDLE;
		break;
		case CONTROLLER_STATE::DRIVE:
			this->Drive_SetHallState(eWheel.BLDCSensor.HallState);
		break;
	}
	
}

void MotorController::SetDuty(uint32_t duty)
{
	TCC0->CC[0].reg	= duty;
}

void MotorController::SetHallState(HALL_STATE state)
{
	if (this->ControllerState == CONTROLLER_STATE::DRIVE)
	{
		this->Drive_SetHallState(state);
	}
}

MotorController::~MotorController()
{
}