/* 
* DiagLink.cpp
*
* Created: 03.06.2020 20:11:20
* Author: dominik helhake
*/
#include "DiagLink.h"
#include "..\Peripheral/System/System.h"
#include "..\Peripheral\CP2102\CP2102.h"
#include "..\Peripheral\W25Q128\W25Q128.h"

DiagLink Diagnostic;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT DiagLink::Run(uint32_t timeStamp)
{	
	if (this->ReceiveBuffer[0] != 0x00)
	{			
		switch (((DIAGNOSTIC_CMD)this->ReceiveBuffer[0]))
		{
			case DIAGNOSTIC_CMD::REQUEST_MODE:
				this->ResponseMode(this->Mode);
			break;
			case DIAGNOSTIC_CMD::SW_RESET:
				if (this->Mode == DIAGNOSTIC_MODE::DIAGNOSTIC &&
					this->ReceiveBuffer[1] == 0xDE &&
					this->ReceiveBuffer[2] == 0xAD &&
					this->ReceiveBuffer[3] == 0xBE &&
					this->ReceiveBuffer[4] == 0xEF)
					{
						GPBR->SYS_GPBR[0] = 0x00000000;
						System.SoftReset();
					}
			break;
			case DIAGNOSTIC_CMD::REQUEST_DIAGNOSTIC_MODE:
				if (this->Mode == DIAGNOSTIC_MODE::TRACE &&
					this->ReceiveBuffer[1] == 0xDE &&
					this->ReceiveBuffer[2] == 0xAD &&
					this->ReceiveBuffer[3] == 0xBE &&
					this->ReceiveBuffer[4] == 0xEF)
					{
						GPBR->SYS_GPBR[0] = 0xDEADBEEF;
						System.SoftReset();
					}
			break;
			case DIAGNOSTIC_CMD::REQUEST_SECTOR_ERASE:
				this->SectorErase((uint16_t)((this->ReceiveBuffer[1] << 8) | (this->ReceiveBuffer[2] << 0)));
			break;
			case DIAGNOSTIC_CMD::REQUEST_FLASH_PAGE:
				this->ResponseFlashPage((uint16_t)((this->ReceiveBuffer[1] << 8) | (this->ReceiveBuffer[2] << 0)));
			break;
			default:
			break;
		}
		
		this->ReceiveBuffer[0] = 0x00;
		return RUN_RESULT::SUCCESS;
	}
	
	return RUN_RESULT::IDLE;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
DiagLink::DiagLink()
{
	this->Mode = DIAGNOSTIC_MODE::TRACE;
}

void DiagLink::ResponseFlashPage(uint16_t pageIndex)
{
	if (this->Mode != DIAGNOSTIC_MODE::DIAGNOSTIC)
		return;
	
	uint8_t data[256] = { 0 };
	DataFlash.PageRead(pageIndex, data);
	
	DiagPort.WriteMessage(DIAGNOSTIC_CMD::RESPONSE_FLASH_PAGE, 256, data);	
}

void DiagLink::SetDiagnosticMode()
{
	this->Mode = DIAGNOSTIC_MODE::DIAGNOSTIC;
}

void DiagLink::ResponseMode(DIAGNOSTIC_MODE mode)
{
	uint8_t data[1] = { (uint8_t)mode };
	
	DiagPort.WriteMessage(DIAGNOSTIC_CMD::RESPONSE_MODE, 1, data);
}

void DiagLink::SectorErase(uint16_t sector)
{
	uint8_t data[1] = { (uint8_t)SECTOR_ERASE_RESULT::ERROR };
	if (this->Mode != DIAGNOSTIC_MODE::DIAGNOSTIC)
	{	
		data[0] = (uint8_t)SECTOR_ERASE_RESULT::INVALID_MODE;
		DiagPort.WriteMessage(DIAGNOSTIC_CMD::RESPONSE_SECTOR_ERASE, 1, data);	
	} else {		
		if (DataFlash.IsBusy())
		{			
			data[0] = (uint8_t)SECTOR_ERASE_RESULT::BUSY_REPEAT;
			DiagPort.WriteMessage(DIAGNOSTIC_CMD::RESPONSE_SECTOR_ERASE, 1, data);
		} else {		
			DataFlash.SectorErase(sector);
			while (DataFlash.IsBusy());
						
			data[0] = (uint8_t)SECTOR_ERASE_RESULT::SUCCESS;
			DiagPort.WriteMessage(DIAGNOSTIC_CMD::RESPONSE_SECTOR_ERASE, 1, data);			
		}
	}	
}