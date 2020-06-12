/* 
* DiagLink.h
*
* Created: 03.06.2020 20:11:20
* Author: dominik helhake
*/
#ifndef __DIAGLINK_H__
#define __DIAGLINK_H__

#include "sam.h"
#include "..\Task.h"

#define DIAGNOSTIC_RCV_BUF_SIZE	128

enum class DIAGNOSTIC_CMD : uint8_t
{
	ENTER_DIAGNOSTIC_MODE = 0x01,
	ERASE_MEMORY	= 0x02,
	REQUEST_MODE	= 0x03,
	RESPONSE_MODE	= 0x04,
	SW_RESET		= 0xFF
};

enum class DIAGNOSTIC_MODE : uint8_t
{
	INVALID = 0x00,
	TRACE = 0x01,
	DIAGNOSTIC = 0x02
};

class DiagLink : public Task
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	virtual RUN_RESULT Run(uint32_t timeStamp);

	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	private:		
		DIAGNOSTIC_MODE Mode;
	
	public:	
		uint8_t ReceiveBufferIndex = 0;
		uint8_t ReceiveBuffer[DIAGNOSTIC_RCV_BUF_SIZE];
		
		DiagLink();
		
		void SetDiagnosticMode();
		void ResponseMode(DIAGNOSTIC_MODE mode);
}; //DiagLink

extern DiagLink Diagnostic;

#endif //__DIAGLINK_H__
