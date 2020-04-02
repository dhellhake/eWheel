/*
 * CC41A.cpp
 *
 * Created: 29.03.2020 16:57:07
 *  Author: Dominik Hellhake
 */
#include <string.h>
#include "CC41A.h"
#include "..\Device\SERCOM\SERCOM.h"
#include "..\..\COMPort\COMPort.h"

#include "..\Device\DSU\DSU.h"

CC41A BLEDevice;

CC41A::CC41A()
{
	
}

void CC41A::SendDataPacket(DataPacket *pkg)
{	
	uint8_t* packet = (uint8_t*)pkg;
	
	//PayLoad
	for (uint8_t x = 0; x < pkg->_length + 4; x++)
		SERCOM1_SendByte(packet[x]);
		
	//CRC32
	uint32_t crc32_act = CalculateCRC32((uint32_t*)pkg->_data, pkg->_length);
	uint8_t* crc32 = (uint8_t*)&crc32_act;
	for (uint8_t x = 0; x < 4; x++)
		SERCOM1_SendByte(crc32[x]);
}

void CC41A::ReceiveByte(uint8_t data)
{
	this->ReceiveBuffer[this->ReceiveBufferIndex] = data;
	this->ReceiveBufferIndex++;
	
	if (this->ReceiveBufferIndex >= 4)
	{
		uint16_t length = 4 + (this->ReceiveBuffer[3] << 8 | this->ReceiveBuffer[2]) + 4;
		
		if(this->ReceiveBufferIndex >= length)
		{			
			this->ReceivedDataPacket(this->ReceiveBuffer);
			this->ReceiveBufferIndex = 0;
		}
	}		
}

void CC41A::ReceivedDataPacket(uint8_t *data)
{
	uint16_t tpe = data[1] << 8 | data[0];
	uint16_t len = data[3] << 8 | data[2];	
	uint32_t crc32 = (uint32_t) (data[len + 4] << 24	| 
								 data[len + 5] << 16	|
								 data[len + 6] << 8		|
								 data[len + 7] << 0);
								 								 
	uint32_t crc32_act = CalculateCRC32((uint32_t*)&data[4], len);
	
	if (crc32 == crc32_act)
	{
		uint8_t* addr;
		if (tpe > 0 && tpe < 128)
		{
			addr = (uint8_t*)&ComLink.ReceivedPacket;
		} else 
			return;
			
		memcpy(addr, data, len + 4);
	}
}