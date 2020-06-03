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

CP2102 TracePort;

void CP2102::EnterTraceMode(uint8_t *data)
{
	if (data[1] == 0xDE &&
		data[2] == 0xAD &&
		data[3] == 0xBE &&
		data[4] == 0xEF)
		Mode = RUN_MODE::TRACE_MODE;
}


void CP2102::ReceiveByte(uint8_t data)
{
	if (this->ReceiveBufferIndex == 0)
		if (data != 0xAA)
			return;
	
	this->ReceiveBuffer[this->ReceiveBufferIndex++] = data;
	
	if (this->ReceiveBufferIndex >= 3)						// at least start byte and length
	{
		uint16_t length = (this->ReceiveBuffer[1] << 8 | this->ReceiveBuffer[2]);
		
		if (this->ReceiveBufferIndex >= length + 4)
		{			
			this->ReceiveBufferIndex = 0;	
			
			if (this->ReceiveBuffer[3] == ((uint8_t)TRACE_CMD::ENTER_TRACEMODE))
				this->EnterTraceMode(&this->ReceiveBuffer[3]);
			else if (this->ReceiveBuffer[3] == ((uint8_t)TRACE_CMD::REQUEST_MODE))
				this->ResponseMode();
			else
				memcpy(Trace.ReceiveBuffer, &this->ReceiveBuffer[3], length + 1);
		}
	}
}

void CP2102::ResponseMode()
{
	uint8_t data[1] = { (uint8_t)Mode };
	
	this->WriteMessage(TRACE_CMD::RESPONSE_MODE, 1, data);
}

void CP2102::WriteMessage(TRACE_CMD cmd, uint8_t length, uint8_t *data)
{
	length = 1;
	uint8_t buffer[4] = 
	{
		0xAA,
		(uint8_t)((length >> 8) & 0xFF),
		(uint8_t)((length >> 0) & 0xFF),
		(uint8_t)cmd
	};
	
	for (uint8_t x = 0; x < 4; x++)
		UART_SendByte(CP2102_UART, buffer[x]);
	for (uint8_t x = 0; x < length; x++)
		UART_SendByte(CP2102_UART, data[x]);
}