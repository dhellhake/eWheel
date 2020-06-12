/* 
* CP2102.h
*
* Created: 27.05.2020 20:06:37
* Author: dominik hellhake
*/
#ifndef __CP2102_H__
#define __CP2102_H__

#include "sam.h"
#include "..\..\DiagLink\DiagLink.h"

#define CP2102_UART			UART1
#define CP2102_RCV_BUF_SIZE		64

class CP2102
{
	public:
		void ReceiveByte(uint8_t data);
		void WriteMessage(DIAGNOSTIC_CMD cmd, uint8_t length, uint8_t *data);
		
	private:
		uint8_t ReceiveBufferIndex = 0;
		uint8_t ReceiveBuffer[CP2102_RCV_BUF_SIZE];
		
		void EnterDiagnosticMode(uint8_t *data);
		void ResponseMode();
};

extern CP2102 DiagPort;

#endif //__CP2102_H__
