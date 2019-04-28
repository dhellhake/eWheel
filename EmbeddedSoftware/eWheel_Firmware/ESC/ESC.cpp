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
	
	return RUN_RESULT::SUCCESS;
}

void ESC::Propagate()
{
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
ESC::ESC()
{
	this->Tar_Duty = 0;
} //ESC


void ESC::ReceiveVESCPackage(uint8_t id, uint8_t *data)
{	
	this->CANRcvBuffer[this->CANRcvBufferIndex]._type = (VESCPackageType)id;
	for (uint8_t x = 0; x < VESC_PACKAGE_SIZE; x++)
		this->CANRcvBuffer[this->CANRcvBufferIndex]._data[x] = data[x];
	
	this->CANRcvBufferIndex++;
	
	if (this->CANRcvBufferIndex >= CAN_RECEIVE_BUFFER_SIZE)
		this->CANRcvBufferIndex = 0;
}