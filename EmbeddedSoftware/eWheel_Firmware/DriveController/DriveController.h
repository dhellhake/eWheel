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
	Balancing = 2,
	Driving = 3	
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
		float AvlRelACPD = 0.0f;
	
		DriveController();
	
	private:
		uint32_t LastTarValueUpdate;
		uint32_t DropOver_Dbnc;
		uint32_t State_Dbnc;
		
		inline bool IsStarting()
		{
			return  VESC.Avl_RPM == 0 &&				//Still standing
					Board.Chassis_Roll <= 35.0f &&		//Not left over
					Board.Chassis_Roll >= -30.0f &&		//Not right over
					Board.Chassis_Pitch >= 20.0f;		//Tail down
		}
		
		inline bool IsDroppedOver()
		{
			return Board.Chassis_Roll > 35.0f ||		// Dropped left over
					Board.Chassis_Roll < -30.0f ||		// Dropped right over
					Board.Chassis_Pitch > 20.0f ||		// Tail touches ground
					Board.Chassis_Pitch < -17.0f;		// Nose touches ground
		}
	
		inline float GetAvlACPD()
		{
			float ang = Board.Chassis_Pitch - 4.0f;
			
			if (ang < 5 && ang > -5)
				return 0.0f;
			else if (ang > 0)
				return ((ang / (20.0f/100.0f)) * -1) / 100.0f;
			else
				return ((ang / (15.0f/100.0f)) * -1) / 100.0f;
		}

}; //DriveController

extern DriveController Drive;

#endif //__DRIVECONTROLLER_H__
