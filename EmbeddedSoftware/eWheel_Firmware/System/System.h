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

#include "..\ADS\ADS.h"
#include "..\AT45DB/AT45DB.h"
#include "..\CC41A/CC41A.h"
#include "..\GP2Y/GP2Y.h"
#include "..\LSM9D/LSM9D.h"
#include "..\SSD1306/SSD1306.h"

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
		ADS AnalogSensor;
		
		AT45DB TraceLink;
		
		CC41A DebugLink;

		GP2Y InfraRedSensor;

		LSM9D Orientation;

		SSD1306 OLED;	
		
	private:
		System( const System &c );
		System& operator=( const System &c );
}; //System

extern System eWheel;

#endif //__SYSTEM_H__
