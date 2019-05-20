/* 
* Chassis.h
*
* Created: 28.04.2019 20:30:50
* Author: Dominik Hellhake
*/
#ifndef __CHASSIS_H__
#define __CHASSIS_H__

#include "samc21.h"
#include "..\Executable.h"
#include "..\LSM9D\LSM9D.h"

class Chassis : public Executable
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	virtual RUN_RESULT Run(uint32_t timeStamp);
	
	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	public:	
		float Chassis_Pitch = 0;
		float Chassis_Roll = 0;
		float Road_Pitch = 0;
		float Road_Roll = 0;
	
		Chassis();
		void SetLED(bool state);
}; //Chassis

extern Chassis Board;

#endif //__CHASSIS_H__
