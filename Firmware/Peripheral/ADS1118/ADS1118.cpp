/* 
* ADS1118.cpp
*
* Created: 13.06.2020 18:30:36
* Author: dominik hellhake
*/
#include "ADS1118.h"
#include "..\SAM4S\SPI\SPIlib.h"

ADS1118 ADS;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT ADS1118::Run(uint32_t timeStamp)
{		
	uint16_t rawValue;
	switch(this->Current_Sampling)
	{
		case ADS_IR_Pos::FRONT:
			rawValue = this->ReadRaw(this->Config_Back);
			this->Qu_Vfront = this->SetVoltage(rawValue, &this->Avl_Vfront);
			this->Current_Sampling = ADS_IR_Pos::BACK;
		break;
		case ADS_IR_Pos::BACK:
			rawValue = this->ReadRaw(this->Config_Front);
			this->Qu_Vback = this->SetVoltage(rawValue, &this->Avl_Vback);
			this->Current_Sampling = ADS_IR_Pos::FRONT;
		break;
	}
	
	if (this->Qu_Vback == ADS_QU_Voltage::VALID && this->Qu_Vfront == ADS_QU_Voltage::VALID)
		return RUN_RESULT::SUCCESS;
	else
		return RUN_RESULT::ERROR;
}

RUN_RESULT ADS1118::Setup(uint32_t timeStamp)
{
	return RUN_RESULT::SUCCESS;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/

uint16_t ADS1118::ReadRaw(uint16_t config)
{
	uint8_t data[2] = { (uint8_t)(config >> 8), (uint8_t)(config & 0xFF) };
	
	SPI_Select(SPI_PCS_PER::ADS11);
	SPI_TransferData(data, 2);
	SPI_Finish();
	
	return (data[0] << 8) | data[1];
}

ADS_QU_Voltage ADS1118::SetVoltage(uint16_t rawValue, float *ref)
{
	if(rawValue>=0x8000)		
		*ref = ((float)(((~rawValue) + 1) * 0.000125f) * -1);
	else
		*ref = (float)(rawValue * 0.000125f);
	
	if (*ref < 3.0f && *ref > 0.4f)
		return ADS_QU_Voltage::VALID;
	else
		return ADS_QU_Voltage::INVALID;
}

ADS1118::ADS1118()
{
	this->Current_Sampling = ADS_IR_Pos::FRONT;
	
	this->Config_Front = (0x1 << 15)		| //Single-shot conversion start
						(0b100 << 12)	| //Input multiplexer
						(0b001 << 9)	| //Gain amplifier (+- 4.096V)
						(0b1 << 8)		| //Device operating mode (1 = Single shot)
						(0b101 << 5)	| //Data rate (100 SPS)
						(0b0 << 4)		| //Temperature sensor mode (0 = ADC mode)
						(0b1 << 3)		| //Pullup enable (1 = floating DOUT)
						(0b01 << 1)		| //No operation
						(0b1 << 0);		  //Reserved
	
	
	this->Config_Back = (0x1 << 15)		| //Single-shot conversion start
						(0b101 << 12)	| //Input multiplexer
						(0b001 << 9)	| //Gain amplifier (+- 4.096V)
						(0b1 << 8)		| //Device operating mode (1 = Single shot)
						(0b101 << 5)	| //Data rate (100 SPS)
						(0b0 << 4)		| //Temperature sensor mode (0 = ADC mode)
						(0b1 << 3)		| //Pullup enable (1 = floating DOUT)
						(0b01 << 1)		| //No operation
						(0b1 << 0);		  //Reserved
}