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
		uint16_t length = 8 + this->ReceivedPacket.Length + 4;
		
		if(this->ReceiveBufferIndex >= length)
		{
			this->ReceivedDataPacket(&this->ReceivedPacket);
			
			this->ReceiveBufferIndex = 0;
			
			for (uint8_t x = 0; x < RECEIVE_BUFFER_SIZE + 12; x++)
				((uint8_t*)&this->ReceivedPacket)[x] = 0x00;
		}
	}
	
	//Data transmission
	if (this->BytesToSend > 64 && this->ClearToSend) //At least two packets to send
	{	
		this->SendDataPacket((uint8_t)this->Command, &this->SendBuffer[this->SendBufferIndex], 64, this->SendBufferIndex, this->SourceSwc, this->DestinationSwc, 0x0);
		this->BytesToSend -= 64;
		this->SendBufferIndex += 64;
		this->ClearToSend = false;
	} else if (this->Command != BLECmd::INVALID && this->ClearToSend) // just one packet to send
	{
		this->SendDataPacket((uint8_t)this->Command, &this->SendBuffer[this->SendBufferIndex], this->BytesToSend, this->SendBufferIndex, this->SourceSwc, this->DestinationSwc, 0x2);
		this->BytesToSend = 0;
		this->SendBufferIndex = 0;
		
		for (uint16_t x = 0; x < SEND_BUFFER_SIZE; x++)
			this->SendBuffer[x] = 0x0;
		this->Command = BLECmd::INVALID;
	}
	
	return RUN_RESULT::SUCCESS;
}

CC41A::CC41A()
{
	for (uint16_t x = 0; x < SEND_BUFFER_SIZE; x++)
		this->SendBuffer[x] = 0x0;
}


void CC41A::SendData(BLECmd cmd, uint8_t *data, uint16_t length, BLESwc sSwc, BLESwc dSwc)
{
	uint16_t len = length % 4;
	if (len != 0)
		len = length + (4 - len);
	else
		len = length;
	
	if (len <= SEND_BUFFER_SIZE)
	{
		this->Command = cmd;
		this->SourceSwc = sSwc;
		this->DestinationSwc = dSwc;
		memcpy(this->SendBuffer, data, len);
		this->BytesToSend = len;
	}
}

void CC41A::SendDataPacket(uint8_t cmd, uint8_t *data, uint16_t length, uint16_t sequence, BLESwc sSwc, BLESwc dSwc, uint8_t flags)
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
	
	//Cmd
	SERCOM1_SendByte(cmd);	
	
	if (length > 0)
	{		
		//CRC32
		uint32_t crc32_act = CalculateCRC32((uint32_t*)data, GetCRC32Length(length));
		
		uint8_t* crc32 = (uint8_t*)&crc32_act;
		for (uint8_t x = 0; x < 4; x++)
			SERCOM1_SendByte(crc32[x]);
					
		//PayLoad
		for (uint8_t x = 0; x < length; x++)
		SERCOM1_SendByte(data[x]);
	} else
	{
		//CRC32
		for (uint8_t x = 0; x < 4; x++)
			SERCOM1_SendByte(0x0);
	}
}

void CC41A::SendACK(BLESwc sSwc, BLESwc dSwc)
{
	this->SendDataPacket((uint8_t)BLECmd::ACK_CMD, NULL, 0, 0, sSwc, dSwc, 0x1);
}


void CC41A::ReceiveByte(uint8_t data)
{
	uint8_t *ptr = (uint8_t*)&this->ReceivedPacket;
	ptr[this->ReceiveBufferIndex] = data;
	this->ReceiveBufferIndex++;		
}

void CC41A::ReceivedDataPacket(BLEPacket *pkt)
{	
	if ((pkt->Flags & 0x1) == 0x1)
		this->ClearToSend = true;
		
	if (pkt->Length > 0)
	{
		uint32_t crc32_act = CalculateCRC32((uint32_t*)pkt->Data, GetCRC32Length(pkt->Length));
				
		if (pkt->CRC32 == crc32_act)
		{
			uint8_t* addr;
			if (pkt->DstSwc == BLESwc::ComLink)
			{
				addr = &ComLink.ReceiveBuffer[pkt->Sequence];
								
				if (pkt->Flags && (uint8_t)BLEPacketFlags::FIN)
					ComLink.ReceivedCommand = pkt->Cmd;
				else
					this->SendACK(pkt->SrcSwc, pkt->DstSwc);
			} else
				return;
			
			memcpy(addr, pkt->Data, pkt->Length);
		}
	}
}