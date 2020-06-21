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
	REQUEST_DIAGNOSTIC_MODE		= 0x01,
	RESPONSE_DIAGNOSTIC_MODE	= 0x02,
	
	REQUEST_SECTOR_ERASE		= 0x03,
	RESPONSE_SECTOR_ERASE		= 0x04,
	
	REQUEST_MODE				= 0x05,
	RESPONSE_MODE				= 0x06,
	
	REQUEST_FLASH_PAGE			= 0x07,
	RESPONSE_FLASH_PAGE			= 0x08,
	
	SW_RESET					= 0xFF
};

enum class DIAGNOSTIC_MODE : uint8_t
{
	INVALID = 0x00,
	TRACE = 0x01,
	DIAGNOSTIC = 0x02
};

enum class SECTOR_ERASE_RESULT : uint8_t
{
	BUSY_REPEAT = 0x01,
	SUCCESS = 0x02,
	INVALID_MODE = 0x03,
	ERROR = 0x04
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
	
		void SectorErase(uint16_t sector);
		void ResponseMode(DIAGNOSTIC_MODE mode);
		void ResponseFlashPage(uint16_t pageIndex);
		
	public:	
		uint8_t ReceiveBufferIndex = 0;
		uint8_t ReceiveBuffer[DIAGNOSTIC_RCV_BUF_SIZE];
		
		DiagLink();
		
		void SetDiagnosticMode();	
}; //DiagLink

extern DiagLink Diagnostic;

#endif //__DIAGLINK_H__
