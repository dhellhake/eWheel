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

#define POS_PID_THR_DEG 	5
#define POS_PID_Kp			0.02f / (float)POS_PID_THR_DEG


enum class DriveState
{
	Stopped = 0,
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
		virtual RUN_RESULT Run(uint32_t timeStamp);
	
	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	public:
		DriveState State;
		float AvlRelACPD = 0.0f;
	
		DriveController();
	
	private:
		float LastCtrlError;
		
		uint32_t State_Dbnc;
		
		uint32_t LastControlCycle = 0;
		
		
		
		inline float GetAvlACPD()
		{			
			if (Board.Chassis_Pitch < 5 && Board.Chassis_Pitch > -5)
				return 0.0f;
			else if (Board.Chassis_Pitch > 0)
				return ((Board.Chassis_Pitch / (20.0f/100.0f)) * -1) / 100.0f;
			else
				return ((Board.Chassis_Pitch / (15.0f/100.0f)) * -1) / 100.0f;
		}

}; //DriveController

extern DriveController Drive;

#endif //__DRIVECONTROLLER_H__
