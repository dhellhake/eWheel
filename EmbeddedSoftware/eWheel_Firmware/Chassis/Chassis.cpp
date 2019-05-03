/* 
* Chassis.cpp
*
* Created: 28.04.2019 20:30:49
* Author: Dominik Hellhake
*/
#include "Chassis.h"

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT Chassis::Run(uint32_t timeStamp)
{
	this->Chassis_Pitch = this->Gyro.Pitch - this->Road_Pitch;
	this->Chassis_Roll = this->Gyro.Roll - this->Road_Pitch;	
	
	this->Status = TASK_STATUS::COMPLETE;
	return RUN_RESULT::SUCCESS;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
Chassis::Chassis()
{
} //Chassis
