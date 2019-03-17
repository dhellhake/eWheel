/* 
* MotorSensor.cpp
*
* Created: 12.05.2018 10:18:16
* Author: Dominik Hellhake
*/
#include "MotorSensor.h"

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT MotorSensor::Run()
{
	
	return RUN_RESULT::SUCCESS;
}

void MotorSensor::Propagate()
{
	if (this->OLED != NULL)
		this->OLED->SetRow(this->StateToIndex(this->HallState), 0);
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
void MotorSensor::EnableInterrupt()
{	
	//Enable Interrupt on ECTINT 7, 12, 13
	EIC->INTENSET.reg |= (1 << 7) | (1 << 12) | (1 << 13);
}