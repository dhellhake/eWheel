/* 
* ESC.cpp
*
* Created: 14.04.2019 15:02:13
* Author: Dominik Hellhake
*/
#include "ESC.h"
#include "..\LowLevel\Device\CAN\CANCont.h"

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT ESC::Run(uint32_t timeStamp)
{	
	if (this->CANRcvBufferIndex > 0)
		this->ProcessVESCPackages();
	else if (timeStamp - this->LastTarValueUpdate >= 100)
		this->SendTarValues(timeStamp);
	else
		return RUN_RESULT::INACTIVE;
	
	this->LastExecuted = timeStamp;
	return RUN_RESULT::SUCCESS;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
ESC::ESC()
{
	this->Tar_Duty = 0.05f;
} //ESC

void ESC::SendTarValues(uint32_t timeStamp)
{
	this->LastTarValueUpdate = timeStamp;
	
	uint8_t data[4] = {0};
	buffer_set_int32(data, ((int32_t)(this->Tar_Duty * 1000.0f * 100.0f)));
		
	CAN_SendExtMessage(124 | ((uint8_t)VESCPackageType::CAN_PACKET_SET_DUTY << 8), data, 4, 0);
}

void ESC::ProcessVESCPackages()
{
	for (uint8_t index = 0; index < this->CANRcvBufferIndex; index++)
	{
		switch (this->CANRcvBuffer[index]._type)
		{
			case VESCPackageType::CAN_PACKET_STATUS:
				this->Avl_RPM =			(float)buffer_get_int32(this->CANRcvBuffer[index]._data, 0);
				this->Avl_Current =		(float)buffer_get_int16(this->CANRcvBuffer[index]._data, 4) / 10.0f;
				this->Avl_Duty =		(float)buffer_get_int16(this->CANRcvBuffer[index]._data, 6) / 1000.0f;
			break;
			case VESCPackageType::CAN_PACKET_STATUS_2:
				this->Avl_Ah =			(float)buffer_get_int32(this->CANRcvBuffer[index]._data, 0) / 1e4;
				this->Avl_AhCharged =	(float)buffer_get_int32(this->CANRcvBuffer[index]._data, 4) / 1e4;
			break;
			case VESCPackageType::CAN_PACKET_STATUS_3:
				this->Avl_Wh =			(float)buffer_get_int32(this->CANRcvBuffer[index]._data, 0) / 1e4;
				this->Avl_WhCharged =	(float)buffer_get_int32(this->CANRcvBuffer[index]._data, 4) / 1e4;
			break;
			case VESCPackageType::CAN_PACKET_STATUS_4:
				this->Avl_TempFET =		(float)buffer_get_int16(this->CANRcvBuffer[index]._data, 0) / 10.0f;
				this->Avl_TempMotor =	(float)buffer_get_int16(this->CANRcvBuffer[index]._data, 2) / 10.0f;
				this->Avl_CurrentIn =	(float)buffer_get_int16(this->CANRcvBuffer[index]._data, 4) / 10.0f;
				this->Avl_PIDPosNow =	(float)buffer_get_int16(this->CANRcvBuffer[index]._data, 6) / 50.0f;
			break;
			default:
			break;
		}
	}
	this->CANRcvBufferIndex = 0;	
}


void ESC::ReceiveVESCPackage(uint8_t id, uint8_t *data)
{	
	this->CANRcvBuffer[this->CANRcvBufferIndex]._type = (VESCPackageType)id;
	for (uint8_t x = 0; x < VESC_PACKAGE_SIZE; x++)
		this->CANRcvBuffer[this->CANRcvBufferIndex]._data[x] = data[x];
	
	this->CANRcvBufferIndex++;
	
	if (this->CANRcvBufferIndex >= CAN_RECEIVE_BUFFER_SIZE)
		this->CANRcvBufferIndex = 0;
}