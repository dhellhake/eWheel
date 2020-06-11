/* 
* TraceLink.cpp
*
* Created: 03.06.2020 20:11:20
* Author: dominik helhake
*/
#include "TraceLink.h"
#include "..\Peripheral/System/System.h"
#include "..\Peripheral\CP2102\CP2102.h"

TraceLink Trace;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT TraceLink::Run(uint32_t timeStamp)
{	
	if (this->ReceiveBuffer[0] != 0x00)
	{			
		switch (((TRACE_CMD)this->ReceiveBuffer[0]))
		{
			case TRACE_CMD::SW_RESET:
				if (Mode == RUN_MODE::TRACE_MODE &&
					this->ReceiveBuffer[1] == 0xDE &&
					this->ReceiveBuffer[2] == 0xAD &&
					this->ReceiveBuffer[3] == 0xBE &&
					this->ReceiveBuffer[4] == 0xEF)
					SoftwareReset();
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
