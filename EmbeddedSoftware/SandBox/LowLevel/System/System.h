/* 
* System.h
*
* Created: 08.05.2018 18:22:30
* Author: dominik hellhake
*/


#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "samc21.h"

class System
{
//variables
public:
	static uint32_t SysTick_Overflow;
protected:
private:

//functions
public:
	static void Init();
	
	static void SetPinPeripheralFunction(uint32_t pinmux);
	
	static uint32_t GetGCLK_Hz(uint32_t gclk_id);
	
	static uint32_t GetElapsedMilis();
		
protected:
private:
	System();
	~System();
	System( const System &c );
	System& operator=( const System &c );

}; //System

#endif //__SYSTEM_H__
