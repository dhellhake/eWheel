/* 
* CC41A.cpp
*
* Created: 09.03.2019 18:06:43
* Author: Dominik Hellhake
*/
#include "CC41A.h"
#include "..\System\System.h"

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT CC41A::Run()
{
	uint8_t cmd = this->ReceiveBuffer[0];
	
	
	switch ((DEBUG_CMD)this->ReceiveBuffer[0])
	{
		case DEBUG_CMD::SetLED:
			eWheel.BLDCController.SetState(CONTROLLER_STATE::DRIVE);
		break;
		case DEBUG_CMD::UnSetLED:
			eWheel.BLDCController.SetState(CONTROLLER_STATE::IDLE);
		break;
	}
		
	//Reset Command Buffer
	this->ReceiveBufferIndex = 0;
	
	return RUN_RESULT::SUCCESS;
}

void CC41A::Propagate()
{
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
CC41A::CC41A()
{
} //CC41A

void CC41A::ReceiveByte(uint8_t data)
{
	this->ReceiveBuffer[this->ReceiveBufferIndex] = data;
	this->ReceiveBufferIndex++;
}