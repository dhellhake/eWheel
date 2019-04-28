/* 
* Orientation.h
*
* Created: 28.04.2019 20:30:50
* Author: Dominik Hellhake
*/
#ifndef __ORIENTATION_H__
#define __ORIENTATION_H__

#include "samc21.h"
#include "..\Executable.h"
#include "..\LSM9D\LSM9D.h"

class Chassis : public Executable
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	virtual bool IsReady(uint32_t timeStamp)
	{
		return this->Gyroscope->Status == TASK_STATUS::COMPLETE;
	}
	virtual RUN_RESULT Run(uint32_t timeStamp);
	
	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	public:
	LSM9D* Gyroscope;
	
	Chassis();
}; //Orientation

#endif //__ORIENTATION_H__
