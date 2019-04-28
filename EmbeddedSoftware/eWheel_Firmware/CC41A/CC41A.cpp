/* 
* CC41A.cpp
*
* Created: 09.03.2019 18:06:43
* Author: Dominik Hellhake
*/
#include "CC41A.h"
#include "..\System\System.h"
#include "..\LowLevel\USART\USART.h"

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT CC41A::Run(uint32_t timeStamp)
{	
	if (this->ReceiveBufferIndex >= 2)
	{
		switch ((DEBUG_CMD)this->ReceiveBuffer[0])
		{
			case DEBUG_CMD::DEBUG_LS9M:
				eWheel.Orientation.TraceEnabled = this->ReceiveBuffer[1] > 0;
			break;
			case DEBUG_CMD::SetLED:
				eWheel.SetLED(this->ReceiveBuffer[1] > 0);
			break;
		}
		//Reset Command Buffer
		this->ReceiveBufferIndex -= 2;
	}
	else
		return RUN_RESULT::INACTIVE;	
	
	this->LastExecuted = timeStamp;
	return RUN_RESULT::SUCCESS;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
CC41A::CC41A()
{
} //CC41A

void CC41A::SendDataPackage(DataPackage *pkg)
{	
	//PackageType
	SERCOM1_SendByte((uint8_t)pkg->_type);
		
	//TimeStamp
	for (uint8_t x = 0; x < 4; x++)
		SERCOM1_SendByte(*(((uint8_t*)&(pkg->_timeStamp)) + x));
		
	//PayLoad
	for (uint8_t x = 0; x < pkg->_length; x++)
		SERCOM1_SendByte(pkg->_data[x]);
}

void CC41A::ReceiveByte(uint8_t data)
{
	this->ReceiveBuffer[this->ReceiveBufferIndex] = data;
	this->ReceiveBufferIndex++;
}