/* 
* MotorDriver.h
*
* Created: 12.05.2018 13:25:12
* Author: dominik hellhake
*/


#ifndef __MOTORDRIVER_H__
#define __MOTORDRIVER_H__

#include "sam.h"
#include "..\..\LowLevel\System\System.h"

class MotorDriver
{
//variables
public:
protected:
private:

//functions
public:
	static void InitTC1();	//ToBeProtected
	static void SetSlowDrive();
	static void SetDrive();
protected:
	MotorDriver();
	~MotorDriver();
private:
	MotorDriver( const MotorDriver &c );
	MotorDriver& operator=( const MotorDriver &c );

}; //MotorDriver

#endif //__MOTORDRIVER_H__
