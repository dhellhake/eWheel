/* 
* CC41A.cpp
*
* Created: 09.03.2019 18:06:43
* Author: Dominik Hellhake
*/
#include "COMPort.h"
#include "..\Chassis\Chassis.h"
#include "..\ESC\ESC.h"
#include "..\DriveController\DriveController.h"
#include "..\LowLevel\Device\SERCOM\SERCOM.h"

#include "..\LowLevel\CC41A\CC41A.h"

COMPort ComLink;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT COMPort::Run(uint32_t timeStamp)
{	
	if (this->ReceivedPacket._type != 0)
	{
		this->ReceivedPacket._type = 0;
		
		DataPacket pkg; 
		pkg._type = 94;
		pkg._length = 4;
		
		for ( uint8_t x = 0; x < 4; x++)
			pkg._data[0] = x;
			
		BLEDevice.SendDataPacket(&pkg);
	}
	
	return RUN_RESULT::SUCCESS;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
COMPort::COMPort()
{
} //CC41A

void COMPort::ReadDriveConfig(uint32_t timeStamp)
{
	DataPacket pkg;
	
	float* fPtr = (float*)pkg._data;
	*fPtr = Drive.Balancing_Kp;
	
	fPtr = (float*)(pkg._data + 4);
	*fPtr = Drive.Balancing_Kd;
	
	pkg._length = 8;
	
	BLEDevice.SendDataPacket(&pkg);
}
void COMPort::WriteDriveConfig(uint8_t *data)
{	
	float* fPtr = (float*)data;	
	Drive.Balancing_Kp = *fPtr;
	
	fPtr = (float*)(data + 4);
	Drive.Balancing_Kd = *fPtr;
}
