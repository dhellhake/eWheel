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
	/* Init drive-channel */
	MotorDriver::InitTCC();
	
	/* Initially Determine electronic motor phase */
	MotorSensor::HallState_Update();
	MotorDriver::Drive_SetPhase(MotorSensor::HallState);
}

void MotorControl::ElectronicPhase_Changed()
{
	/* Process changes through sensor */
	MotorSensor::HallState_Update();
	
	MotorDriver::Drive_SetPhase(MotorSensor::HallState);
}

// default destructor
MotorControl::~MotorControl()
{
} //~MotorControl
