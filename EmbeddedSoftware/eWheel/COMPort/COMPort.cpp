/* 
* CC41A.cpp
*
* Created: 09.03.2019 18:06:43
* Author: Dominik Hellhake
*/
#include <string.h>
#include "COMPort.h"

#include "..\LowLevel\CC41A\CC41A.h"
#include "..\LowLevel\AT45DB\AT45DB.h"

#include "..\DriveController\DriveController.h"

COMPort ComLink;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT COMPort::Run(uint32_t timeStamp)
{	
	if (this->PacketReceived)
	{
		switch ((ComPortCMD)this->ReceiveBuffer[0])
		{
			case ComPortCMD::READ_FLASHPAGE:
				this->ReadFlashPage(this->ReceiveBuffer[1] << 8 | this->ReceiveBuffer[2]);
			break;
			case ComPortCMD::WRITE_FLASHPAGE:
				this->WriteFlashPage(this->ReceiveBuffer[1] << 8 | this->ReceiveBuffer[2], &this->ReceiveBuffer[3]);
			default:
			break;
		}	
		
		this->PacketReceived = false;		
	}
	
	
	return RUN_RESULT::SUCCESS;
}

void COMPort::ReadFlashPage(uint16_t pageIndex)
{	
	FlashPage page;
	
	AT45DB::MemPage_Read(pageIndex, &page);
	
	BLEDevice.SendData(page.Data, FLASHPAGE_SIZE, BLESwc::ComLink, BLESwc::SELF);
}

void COMPort::WriteFlashPage(uint16_t pageIndex, uint8_t *data)
{
	FlashPage page;
	memcpy(page.Data, data, FLASHPAGE_SIZE);
	
	AT45DB::MemPage_Write(pageIndex, &page, true);
	
	uint8_t response[1] = { (uint8_t)ComPortCMD::ACK_CMD };
	BLEDevice.SendData(response, 1, BLESwc::ComLink, BLESwc::SELF); 
}


/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
COMPort::COMPort()
{
} //CC41A
