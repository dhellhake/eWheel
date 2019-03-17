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
RUN_RESULT MotorSensor::Run(uint32_t timeStamp)
{
	float scaled = (((float)(this->HallStateChanged)) / 90);
	this->HallStateChanged = 0;
	
	float elapsed = ((float)timeStamp - (float)this->LastRPMMeasure);
	this->LastRPMMeasure = timeStamp;
	
	this->RPM = 1 / ((elapsed * scaled) / 1000.0f);
	
	return RUN_RESULT::SUCCESS;
}

void MotorSensor::Propagate()
{
	if (this->OLED != NULL)
		this->OLED->SetRow(this->RPM * 100.0f, 0);
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
void MotorSensor::EnableInterrupt()
{	
	//Enable Interrupt on ECTINT 7, 12, 13
	EIC->INTENSET.reg |= (1 << 7) | (1 << 12) | (1 << 13);
}