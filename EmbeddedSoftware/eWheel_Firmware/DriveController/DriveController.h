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

enum class DriveState
{
	DroppedOver = 0,
	Starting = 1,
	Driving = 2	
};

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
		DriveState State;
	
		DriveController();
	
	private:
		uint32_t State_Dbnc;
		inline bool IsStarting()
		{
			return  VESC.Avl_RPM == 0 &&				//Still standing
					Board.Chassis_Roll <= 35.0f &&		//Not left over
					Board.Chassis_Roll >= -30.0f &&		//Not right over
					Board.Chassis_Pitch >= 22.0f;		//Tail down
		}
		
		inline bool IsDroppedOver()
		{
			return Board.Chassis_Roll > 35.0f ||		// Dropped left over
					Board.Chassis_Roll < -30.0f ||		// Dropped right over
					Board.Chassis_Pitch > 22.0f ||		// Tail touches ground
					Board.Chassis_Pitch < -15.0f;		// Nose touches ground
		}

}; //DriveController

extern DriveController Drive;

#endif //__DRIVECONTROLLER_H__
