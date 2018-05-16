/* 
* MotorControl.cpp
*
* Created: 12.05.2018 20:06:55
* Author: dominik hellhake
*/


#include "MotorControl.h"

// default constructor
MotorControl::MotorControl()
{
} //MotorControl

void MotorControl::Init()
{
	/* Init hall-sensor interrupts */
	MotorSensor::InitEIC();
	/* Init U drive-channel */
	MotorDriver::InitTC1();
	/* Init V drive-channel */
	MotorDriver::InitTC0();
	/* Init W drive-channel */
	MotorDriver::InitTC3();
	
	/* Determine electronic motor phase */
	MotorSensor::HallState_Update();
}

void MotorControl::ElectronicPhase_Changed()
{
	/* Process changes through sensor */
	MotorSensor::HallState_Update();
	
	switch (MotorSensor::HallState)
	{
		case HALL_STATE::HALL_STATE_1:
			MotorDriver::SetUHigh();
			MotorDriver::SetVLow();
			MotorDriver::SetWOpen();
		break;
		case HALL_STATE::HALL_STATE_2:
			MotorDriver::SetUHigh();
			MotorDriver::SetVOpen();
			MotorDriver::SetWLow();
		break;
		case HALL_STATE::HALL_STATE_3:
			MotorDriver::SetUOpen();
			MotorDriver::SetVHigh();
			MotorDriver::SetWLow();
		break;
		case HALL_STATE::HALL_STATE_4:
			MotorDriver::SetULow();
			MotorDriver::SetVHigh();
			MotorDriver::SetWOpen();
		break;
		case HALL_STATE::HALL_STATE_5:
			MotorDriver::SetULow();
			MotorDriver::SetVOpen();
			MotorDriver::SetWHigh();
		break;
		case HALL_STATE::HALL_STATE_6:
			MotorDriver::SetUOpen();
			MotorDriver::SetVLow();
			MotorDriver::SetWHigh();
		break;
		case HALL_STATE::UNDEFINED_1:
		case HALL_STATE::UNDEFINED_2:
			MotorDriver::SetUOpen();
			MotorDriver::SetVOpen();
			MotorDriver::SetWOpen();
		break;
	}	
}

// default destructor
MotorControl::~MotorControl()
{
} //~MotorControl
