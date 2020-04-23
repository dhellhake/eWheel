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
	if (this->ReceivedCommand != BLECmd::INVALID)
	{
		switch (this->ReceivedCommand)
		{
			case BLECmd::REQUEST_FLASHPAGE:
				this->ResponseFlashPage(this->ReceiveBuffer[1] << 8 | this->ReceiveBuffer[0]);
			break;
			
			case BLECmd::SET_FLASHPAGE:
				this->SetFlashPage(this->ReceiveBuffer[1] << 8 | this->ReceiveBuffer[0], &this->ReceiveBuffer[3]);
			break;
			
			case BLECmd::REQUEST_DRIVECONFIG:
				this->RequestDriveConfig();
			break;
			
			default:
			break;
		}	
		
		this->ReceivedCommand = BLECmd::INVALID;	
	}
	
	return RUN_RESULT::SUCCESS;
}

void COMPort::RequestDriveConfig()
{
	uint8_t data[8] = {0};

	float* fPtr = (float*)data;
	*fPtr = Drive.Balancing_Kp;

	fPtr = (float*)(data + 4);
	*fPtr = Drive.Balancing_Kd;
	
	BLEDevice.SendData(BLECmd::RESPONSE_DRIVECONFIG, data, 8, BLESwc::ComLink, BLESwc::SELF);
}

void COMPort::ResponseFlashPage(uint16_t pageIndex)
{	
	FlashPage page;
	
	AT45DB::MemPage_Read(pageIndex, &page);
	
	BLEDevice.SendData(BLECmd::RESPONSE_FLASHPAGE, page.Data, FLASHPAGE_SIZE, BLESwc::ComLink, BLESwc::SELF);
}

void COMPort::SetFlashPage(uint16_t pageIndex, uint8_t *data)
{
	FlashPage page;
	memcpy(page.Data, data, FLASHPAGE_SIZE);
	
	AT45DB::MemPage_Write(pageIndex, &page, true);
	
	BLEDevice.SendData(BLECmd::ACK_CMD, NULL, 0, BLESwc::ComLink, BLESwc::SELF); 
}


/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
COMPort::COMPort()
{
} //CC41A
