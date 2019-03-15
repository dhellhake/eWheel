/* 
* ADS.cpp
*
* Created: 09.07.2018 21:35:27
* Author: Dominik Hellhake
*/
#include "ADS.h"

#include "..\LowLevel\SPI\SPI.h"

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT ADS::Run()
{
	uint8_t config[2];
	config[0] = 0b01000010;
	config[1] = 0b11100011;
	
	switch(this->State)
	{
		case ADS_STATE::MotorCurent:
		config[0] = Distance_Config_Val;
		this->MotorCurrent_RAW = (this->WriteRegister(config) * 0.125);
		this->State = ADS_STATE::Distance;
		break;
		case ADS_STATE::Distance:
		config[0] = Battery_Config_Val;
		this->Distance_RAW = (this->WriteRegister(config) * 0.125);
		this->State = ADS_STATE::Battery;
		break;
		case ADS_STATE::Battery:
		config[0] = MotorCurrent_Config_Val;
		this->Battery = (this->WriteRegister(config) * 0.125);
		this->State = ADS_STATE::MotorCurent;
		break;
	}
	
	this->LastRun = GetElapsedMilis();
	
	return RUN_RESULT::SUCCESS;
}

void ADS::Propagate()
{
	if (this->IR != NULL)
	this->IR->SetDistanceRaw(this->Distance_RAW);
	
	if (this->OLED != NULL)
	{
		if (this->Distance_RAW < 0)
		this->OLED->SetRow(this->Distance_RAW * -1, 1);
		else
		this->OLED->SetRow(this->Distance_RAW, 1);
		
		if (this->MotorCurrent_RAW < 0)
		this->OLED->SetRow(this->MotorCurrent_RAW * -1, 0);
		else
		this->OLED->SetRow(this->MotorCurrent_RAW, 0);
	}
}


/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
ADS::ADS()
{
} //ADS

int16_t ADS::WriteRegister(uint8_t *data)
{
	uint8_t result[2] = {0x00, 0x00};
	
	//CS Low
	PORT->Group[0].OUTCLR.reg = PORT_PA02;	
	
	result[0] = SPIPort::SERCOM2_TransmitByte(data[0]);
	result[1] = SPIPort::SERCOM2_TransmitByte(data[1]);
	
	//CS High
	PORT->Group[0].OUTSET.reg = PORT_PA02;
	
	return ((result[0] << 8) | result[1]);
}





