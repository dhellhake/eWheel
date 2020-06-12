/* 
* DiagLink.cpp
*
* Created: 03.06.2020 20:11:20
* Author: dominik helhake
*/
#include "DiagLink.h"
#include "..\Peripheral/System/System.h"
#include "..\Peripheral\CP2102\CP2102.h"

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
			case DIAGNOSTIC_CMD::ENTER_DIAGNOSTIC_MODE:
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

void DiagLink::SetDiagnosticMode()
{
	this->Mode = DIAGNOSTIC_MODE::DIAGNOSTIC;
	this->ResponseMode(this->Mode);
}

void DiagLink::ResponseMode(DIAGNOSTIC_MODE mode)
{
	uint8_t data[1] = { (uint8_t)mode };
	
	DiagPort.WriteMessage(DIAGNOSTIC_CMD::RESPONSE_MODE, 1, data);
}