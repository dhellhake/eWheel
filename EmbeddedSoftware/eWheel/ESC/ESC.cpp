/* 
* ESC.cpp
*
* Created: 14.04.2019 15:02:13
* Author: Dominik Hellhake
*/
#include <string.h>
#include "sam.h"
#include "ESC.h"
#include "..\Peripheral\SAM4S\USART\USARTlib.h"
#include "..\Utilities.h"

ESC VESC;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT ESC::Run(uint32_t timeStamp)
{	
	RUN_RESULT result = RUN_RESULT::IDLE;
		
	if (this->ComState == VESC_COM_STATE::Received)
		this->ReceiveVESCPacket(&this->ReceiveBuffer[2], this->ReceiveBuffer[1] - 1);
		
	if (timeStamp - this->VESCValues_Tstmp >= 100 
		&& this->ComState == VESC_COM_STATE::Waiting)
	{	
		GetVESCValues();
		this->VESCValues_Tstmp = timeStamp;	
		result = RUN_RESULT::SUCCESS;
	}
	
	
	
	return result;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
ESC::ESC()
{
} //ESC

void ESC::Stop()
{
	if (this->Tar_Duty > 0.0f)
		this->SetDuty(0.0f);
	
	this->ResetTarValues();
}

void ESC::SetDuty(float duty_val)
{			
	uint8_t data[4] = { 0 };
	buffer_set_int32(data, ((int32_t)(duty_val * 1000.0f * 100.0f)));
	
	SendVESCPacket(VESCPackageType::COMM_SET_DUTY, data, 4);
}


void ESC::SetCurrent(float current_val)
{
	uint8_t data[4] = { 0 };
	buffer_set_int32(data, ((int32_t)(current_val * 1000.0f)));
	
	SendVESCPacket(VESCPackageType::COMM_SET_HANDBRAKE, data, 4);
}

void ESC::GetVESCValues()
{
	this->ComState = VESC_COM_STATE::Requested;
	SendVESCPacket(VESCPackageType::COMM_GET_VALUES, NULL, 0);
}


void ESC::ReceiveByte(uint8_t data)
{	
	this->ReceiveBuffer[this->ReceiveBufferIndex++] = data;
	
	if (this->ReceiveBufferIndex >= 2 &&						// at least start byte and length
		this->ReceiveBuffer[0] == 0x2 &&						// start byte
		this->ReceiveBuffer[this->ReceiveBuffer[1] + 4] == 0x3)	// end byte
	{
		this->ReceiveBufferIndex = 0;
		this->ReceiveBuffer[this->ReceiveBuffer[1] + 4] = 0;	// delete stop byte
		this->ComState = VESC_COM_STATE::Received;
	}
}

void ESC::ReceiveVESCPacket(uint8_t* payload, uint16_t length)
{	
	switch(((VESCPackageType)payload[0]))
	{
		case VESCPackageType::COMM_GET_VALUES:
			this->Avl_TempFET =		buffer_get_float16(&payload[1], 1e1);
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
	
	this->ComState = VESC_COM_STATE::Waiting;
}

void ESC::SendVESCPacket(VESCPackageType type, uint8_t* payload, uint16_t length)
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

