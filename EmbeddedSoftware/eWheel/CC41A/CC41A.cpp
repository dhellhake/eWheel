/* 
* CC41A.cpp
*
* Created: 09.03.2019 18:06:43
* Author: Dominik Hellhake
*/
#include "CC41A.h"
#include "..\Chassis\Chassis.h"
#include "..\ESC\ESC.h"
#include "..\DriveController\DriveController.h"
#include "..\LowLevel\USART\USART.h"

CC41A Bluetooth;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT CC41A::Run(uint32_t timeStamp)
{	
	if (this->ReceiveBufferIndex >= 2)
	{
		switch ((DEBUG_CMD)this->ReceiveBuffer[0])
		{
			case DEBUG_CMD::SetLED:
				Board.SetLED(this->ReceiveBuffer[1] > 0);
			break;
		}
		//Reset Command Buffer
		this->ReceiveBufferIndex -= 2;
	}	
	
	if (timeStamp - this->LastReported >= 100)
	{
		SendESCTrace(timeStamp);
		SendChassisTrace(timeStamp);
		SendDriveTrace(timeStamp);
		
		this->LastReported = timeStamp;
	}
	
	return RUN_RESULT::SUCCESS;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
CC41A::CC41A()
{
} //CC41A


void CC41A::SendDriveTrace(uint32_t timeStamp)
{
	TracePackage pkg;
	
	uint8_t data[8] = {0};
	
	data[0] = (uint8_t)Drive.State;
	
	float* fPtr = (float*)(data + 4);
	*fPtr = Drive.AvlRelACPD;	
	
	pkg._timeStamp = timeStamp;
	pkg._type = PageType::DriveTrace;
	pkg._data = data;
	pkg._length = 8;
	
	this->SendDataPackage(&pkg);
}


void CC41A::SendESCTrace(uint32_t timeStamp)
{
	TracePackage pkg;
	
	uint8_t data[20] = {0};
	
	float* fPtr = (float*)data;
	*fPtr = VESC.Avl_RPM;
	
	fPtr = (float*)(data + 4);
	*fPtr = VESC.Avl_Duty;
	
	fPtr = (float*)(data + 8);
	*fPtr = VESC.Avl_TempFET;
	
	fPtr = (float*)(data + 12);
	*fPtr = VESC.Tar_Duty;
	
	fPtr = (float*)(data + 16);
	*fPtr = VESC.Avl_PIDPosNow;
	
	pkg._timeStamp = timeStamp;
	pkg._type = PageType::VESCTrace;
	pkg._data = data;
	pkg._length = 20;
	
	this->SendDataPackage(&pkg);
}

void CC41A::SendChassisTrace(uint32_t timeStamp)
{
	TracePackage pkg;
		
	uint8_t data[16] = {0};
		
	float* fPtr = (float*)data;
	*fPtr = Board.Chassis_Pitch;
		
	fPtr = (float*)(data + 4);
	*fPtr = Board.Chassis_Roll;
	
	fPtr = (float*)(data + 8);
	*fPtr = Board.Road_Pitch;
	
	fPtr = (float*)(data + 12);
	*fPtr = Board.Road_Roll;
		
	pkg._timeStamp = timeStamp;
	pkg._type = PageType::BoardTrace;
	pkg._data = data;
	pkg._length = 16;
		
	this->SendDataPackage(&pkg);
}

void CC41A::SendDataPackage(TracePackage *pkg)
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