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


/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT CC41A::Run(uint32_t timeStamp)
{
	//Data retrieval	
	if (this->ReceiveBufferIndex >= 4)
	{
		uint16_t length = 8 + (this->ReceiveBuffer[4] << 8 | this->ReceiveBuffer[5]) + 4;
		
		if(this->ReceiveBufferIndex >= length)
		{
			this->ReceivedDataPacket(this->ReceiveBuffer);
			this->ReceiveBufferIndex = 0;
		}
	}
	
	//Data transmission
	if (this->BytesToSend > 64 && this->ClearToSend) //At least two packets to send
	{	
		this->SendDataPacket(&this->SendBuffer[this->SendBufferIndex], 64, this->SendBufferIndex, this->SourceSwc, this->DestinationSwc, 0x0);
		this->BytesToSend -= 64;
		this->SendBufferIndex += 64;
		this->ClearToSend = false;
	} else if (this->BytesToSend > 0 && this->ClearToSend) // just one packet to send
	{
		this->SendDataPacket(&this->SendBuffer[this->SendBufferIndex], this->BytesToSend, this->SendBufferIndex, this->SourceSwc, this->DestinationSwc, 0x2);
		this->BytesToSend = 0;
		this->SendBufferIndex = 0;
		
		for (uint16_t x = 0; x < SEND_BUFFER_SIZE; x++)
			this->SendBuffer[x] = 0x0;
	}
	
	return RUN_RESULT::SUCCESS;
}

CC41A::CC41A()
{
	for (uint16_t x = 0; x < SEND_BUFFER_SIZE; x++)
		this->SendBuffer[x] = 0x0;
}


void CC41A::SendData(uint8_t *data, uint16_t length, BLESwc sSwc, BLESwc dSwc)
{
	uint16_t len = length % 4;
	if (len != 0)
		len = length + (4 - len);
	else
		len = length;
	
	if (len <= SEND_BUFFER_SIZE)
	{
		this->SourceSwc = sSwc;
		this->DestinationSwc = dSwc;
		memcpy(this->SendBuffer, data, len);
		this->BytesToSend = len;
	}
}

void CC41A::SendDataPacket(uint8_t *data, uint16_t length, uint16_t sequence, BLESwc sSwc, BLESwc dSwc, uint8_t flags)
{		
	//Source Swc
	SERCOM1_SendByte((uint8_t)sSwc);
	
	//Destination Swc
	SERCOM1_SendByte((uint8_t)dSwc);
	
	//Sequence
	SERCOM1_SendByte((sequence >> 8) & 0xFF);
	SERCOM1_SendByte((sequence >> 0) & 0xFF);
	
	//Length
	SERCOM1_SendByte((length >> 8) & 0xFF);
	SERCOM1_SendByte((length >> 0) & 0xFF);
	
	//Flags
	SERCOM1_SendByte((uint8_t)flags);
	
	//reserved
	SERCOM1_SendByte(0xAA);	
	
	if (length > 0)
	{	
		//PayLoad
		for (uint8_t x = 0; x < length; x++)
			SERCOM1_SendByte(data[x]);
	
		//CRC32
		uint32_t crc32_act = CalculateCRC32((uint32_t*)data, length	);
		uint8_t* crc32 = (uint8_t*)&crc32_act;
		for (uint8_t x = 0; x < 4; x++)
			SERCOM1_SendByte(crc32[x]);
	} else
	{
		//CRC32
		for (uint8_t x = 0; x < 4; x++)
			SERCOM1_SendByte(0x0);
	}
}

void CC41A::SendACK(BLESwc sSwc, BLESwc dSwc)
{
	this->SendDataPacket(NULL, 0, 0, sSwc, dSwc, 0x1);
}


void CC41A::ReceiveByte(uint8_t data)
{
	this->ReceiveBuffer[this->ReceiveBufferIndex] = data;
	this->ReceiveBufferIndex++;		
}

void CC41A::ReceivedDataPacket(uint8_t *data)
{
	BLESwc srcSwc =	(BLESwc)data[0];
	BLESwc dstSwc =	(BLESwc)data[1];
	uint32_t seq = (data[2] << 8 | data[3]);
	uint8_t len =		data[4] << 8 | data[5];
	uint8_t flags =     data[6];
	uint32_t crc32 =	(uint32_t) (data[len + 8] << 24	| 
									data[len + 9] << 16	|
									data[len + 10] << 8	|
									data[len + 11] << 0);
	
	if ((flags & 0x1) == 0x1)
		this->ClearToSend = true;
		
	if (len > 0)
	{
		uint32_t crc32_act = CalculateCRC32((uint32_t*)&data[8], len);
		
		if (crc32 == crc32_act)
		{
			uint8_t* addr;
			if (dstSwc == BLESwc::ComLink)
			{
				addr = &ComLink.ReceiveBuffer[seq];
								
				if (flags && (uint8_t)BLEPacketFlags::FIN)
					ComLink.PacketReceived = true;
				else
					this->SendACK(srcSwc, dstSwc);
			} else
				return;
			
			memcpy(addr, &data[8], len);
		}
	}
}