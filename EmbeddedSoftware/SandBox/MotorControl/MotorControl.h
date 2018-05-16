/* 
* MotorControl.h
*
* Created: 12.05.2018 20:06:55
* Author: dominik hellhake
*/


#ifndef __MOTORCONTROL_H__
#define __MOTORCONTROL_H__

#include "MotorDriver/MotorDriver.h"
#include "MotorSensor/MotorSensor.h"

class MotorControl
{
//variables
public:
protected:
private:

//functions
public:
	static void Init();

	static void ElectronicPhase_Changed();
protected:
private:
	MotorControl();
	~MotorControl();
	MotorControl( const MotorControl &c );
	MotorControl& operator=( const MotorControl &c );

}; //MotorControl

#endif //__MOTORCONTROL_H__
