/* 
* DriveController.h
*
* Created: 04.05.2019 09:39:37
* Author: Dominik Hellhake
*/
#ifndef __DRIVECONTROLLER_H__
#define __DRIVECONTROLLER_H__

#include "..\Executable.h"
#include "..\Chassis\Chassis.h"
#include "..\ESC\ESC.h"


class DriveController : public Executable
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	public:
	virtual bool IsReady(uint32_t timeStamp)
	{
		return	Board.TaskStatus == TASK_STATUS::COMPLETE;
	}
	virtual RUN_RESULT Run(uint32_t timeStamp);
	
	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	public:
		DriveController();

}; //DriveController

extern DriveController Drive;

#endif //__DRIVECONTROLLER_H__
