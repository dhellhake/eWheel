/* 
* System.h
*
* Created: 08.05.2018 18:22:30
* Author: Dominik Hellhake
*/
#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "samc21.h"
#include "..\Executable.h"

#include "..\AT45DB/AT45DB.h"
#include "..\CC41A/CC41A.h"
#include "..\LSM9D/LSM9D.h"
#include "..\ESC\ESC.h"

#define TASKPOOL_SIZE 6

class System
{
	public:
		System();
				
		void SetLED(bool state);	
	
	/************************************************************************/
	/* System-Components	                                                */
	/************************************************************************/	
	public:		
		AT45DB TraceLink;
		
		CC41A DebugLink;

		LSM9D Orientation;
		
		ESC vESC;
		
	private:
		System( const System &c );
		System& operator=( const System &c );
}; //System

extern System eWheel;

#endif //__SYSTEM_H__
