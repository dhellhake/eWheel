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
	
	Board.ResetTask();
		
	this->TaskStatus = TASK_STATUS::COMPLETE;
	return RUN_RESULT::SUCCESS;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
DriveController::DriveController()
{
} //DriveController