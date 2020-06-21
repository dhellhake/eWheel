/* 
* CP2102.cpp
*
* Created: 27.05.2020 20:06:37
* Author: dominik hellhake
*/
#include <string.h>
#include "CP2102.h"
#include "..\SAM4S\UART\UARTlib.h"
#include "..\System\System.h"

CP2102 DiagPort;

void CP2102::ReceiveByte(uint8_t data)
{
	if (this->ReceiveBufferIndex == 0)
		if (data != 0xAA)
			return;
	
	this->ReceiveBuffer[this->ReceiveBufferIndex++] = data;
	
	if (this->ReceiveBufferIndex >= 3)
	{
		uint16_t length = (this->ReceiveBuffer[1] << 8 | this->ReceiveBuffer[2]);
		
		if (this->ReceiveBufferIndex >= length + 4)
		{			
			this->ReceiveBufferIndex = 0;
			memcpy(Diagnostic.ReceiveBuffer, &this->ReceiveBuffer[3], length + 1);
		}
	}
}

void CP2102::WriteMessage(DIAGNOSTIC_CMD cmd, uint16_t length, uint8_t *data)
{
	uint8_t buffer[4] = 
	{
		0xAA,
		(uint8_t)((length >> 8) & 0xFF),
		(uint8_t)((length >> 0) & 0xFF),
		(uint8_t)cmd
	};
	
	for (uint8_t x = 0; x < 4; x++)
		UART_SendByte(CP2102_UART, buffer[x]);
	for (uint16_t x = 0; x < length; x++)
		UART_SendByte(CP2102_UART, data[x]);
}