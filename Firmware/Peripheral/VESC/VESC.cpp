/* 
* VESC.cpp
*
* Created: 21.05.2020 15:10:08
* Author: dominik hellhake
*/
#include <string.h>
#include "VESC.h"
#include "..\..\Utilities.h"
#include "..\SAM4S\USART\USARTlib.h"

#include "..\System\System.h"

VESC ESC;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT VESC::Run(uint32_t timeStamp)
{
	RUN_RESULT result = RUN_RESULT::IDLE;
	
	if (this->Tar_Duty != 0.0f)
		this->SetDuty(this->Tar_Duty);
	
	if (this->ComState == VESC_COM_STATE::Received)
	{				
		this->RequestTstmp = timeStamp;
		this->ComState = VESC_COM_STATE::Requested;
		SendVESCPacket(VESCPackageType::COMM_GET_VALUES, NULL, 0);
	} else {
		if (this->RequestTstmp + 50 < timeStamp)
			this->ComState = VESC_COM_STATE::Received;		
	}
	
	return result;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
void VESC::SetDuty(float duty_val)
{
	uint8_t data[4] = { 0 };
	buffer_set_int32(data, ((int32_t)(duty_val * 1000.0f * 100.0f)));
	
	SendVESCPacket(VESCPackageType::COMM_SET_DUTY, data, 4);
}

void VESC::SetCurrent(float current_val)
{
	uint8_t data[4] = { 0 };
	buffer_set_int32(data, ((int32_t)(current_val * 1000.0f)));
	
	SendVESCPacket(VESCPackageType::COMM_SET_HANDBRAKE, data, 4);
}

void VESC::ReceiveByte(uint8_t data)
{
	if (this->ReceiveBufferIndex == 0 && data != 0x02)
		return;
		
	this->ReceiveBuffer[this->ReceiveBufferIndex++] = data;
		
	if (this->ReceiveBufferIndex >= 2 &&
		this->ReceiveBufferIndex > this->ReceiveBuffer[1] + 4)
	{
		if (this->ReceiveBuffer[this->ReceiveBuffer[1] + 4] == 0x3)
		{
			this->ReceiveBufferIndex = 0;
			this->UnpackVESCPacket(&this->ReceiveBuffer[2], this->ReceiveBuffer[1] - 1);
			this->ComState = VESC_COM_STATE::Received;
		} else {
			this->ReceiveBufferIndex = 0;
		}
	}
}

void VESC::UnpackVESCPacket(uint8_t* payload, uint16_t length)
{
	switch(((VESCPackageType)payload[0]))
	{
		case VESCPackageType::COMM_GET_VALUES:
			this->Avl_Temp_ESC =		buffer_get_float16(&payload[1], 1e1);
			this->Avl_Current =		buffer_get_float32(&payload[1 + 4], 1e2);
			this->Avl_CurrentIn =	buffer_get_float32(&payload[1 + 8], 1e2);
			this->Avl_Duty =		buffer_get_float16(&payload[1 + 20], 1e3);
			this->Avl_RPM =			buffer_get_float32(&payload[1 + 22], 1e0);
			this->Avl_Vin =			buffer_get_float16(&payload[1 + 26], 1e1);
			this->Avl_Ah =			buffer_get_float32(&payload[1 + 28], 1e4);
			this->Avl_AhCharged =	buffer_get_float32(&payload[1 + 32], 1e4);
			this->Avl_Wh =			buffer_get_float32(&payload[1 + 36], 1e4);
			this->Avl_WhCharged =	buffer_get_float32(&payload[1 + 40], 1e4);
			this->Avl_Tach	=		buffer_get_int32(&payload[1 + 44]);
			this->FaultCode =		(VESCFaultCode)payload[1 + 52];		
		break;
		default:
		break;
	}
}

void VESC::SendVESCPacket(VESCPackageType type, uint8_t* payload, uint16_t length)
{
	uint8_t packet[256] = { 0 };
	
	packet[0] = 2;				//Short packet indicator
	packet[1] = length + 1;		//Payload-length
	packet[2] = (uint8_t)type;	//PacketType
	packet[length + 5] = 3;		//Packet-End indicator
	
	memcpy(&packet[3], payload, length);
	
	uint16_t crc16 = CRC16(&packet[2], length + 1);
	packet[length + 3] = (uint8_t)(crc16 >> 8);
	packet[length + 4] = (uint8_t)(crc16 & 0xFF);
	
	for (int x = 0; x < length + 6; x++)
		USART_SendByte(VESC_USART, packet[x]);
}