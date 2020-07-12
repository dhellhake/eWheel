/*
 * CC41A.cpp
 *
 * Created: 29.03.2020 16:57:07
 *  Author: Dominik Hellhake
 */
#include <string.h>
#include "CC41A.h"
#include "..\SAM4S\UART\UARTlib.h"

CC41A BLEPort;

CC41A::CC41A()
{
}

void CC41A::ReceiveByte(uint8_t data)
{
	if (this->ReceiveBufferIndex == 0)
		if (data != 0xAA)
			return;
	
	this->ReceiveBuffer[this->ReceiveBufferIndex++] = data;
	
	if (this->ReceiveBufferIndex >= 3)
	{
		uint16_t length = (this->ReceiveBuffer[1] << 8 | this->ReceiveBuffer[2]);
		
		if (length >= CC41A_RCV_BUF_SIZE)
		{
			this->ReceiveBufferIndex = 0;
			return;
		}
		
		if (this->ReceiveBufferIndex >= length + 4)
		{
			this->ReceiveBufferIndex = 0;
			memcpy(BLE.ReceiveBuffer, &this->ReceiveBuffer[3], length + 1);
		}
	}
}

void CC41A::WriteMessage(BLE_CMD cmd, uint16_t length, uint8_t *data)
{
	uint8_t buffer[4] =
	{
		0xAA,
		(uint8_t)((length >> 8) & 0xFF),
		(uint8_t)((length >> 0) & 0xFF),
		(uint8_t)cmd
	};
	
	for (uint8_t x = 0; x < 4; x++)
		UART_SendByte(CC41A_UART, buffer[x]);
	for (uint16_t x = 0; x < length; x++)
		UART_SendByte(CC41A_UART, data[x]);
}