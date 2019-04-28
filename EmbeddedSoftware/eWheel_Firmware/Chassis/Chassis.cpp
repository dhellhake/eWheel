/* 
* Orientation.cpp
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
	
	
	
	this->Status = TASK_STATUS::COMPLETE;
	this->LastExecuted = timeStamp;
	return RUN_RESULT::SUCCESS;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
Chassis::Chassis()
{
} //Orientation
