/* 
* BLELink.cpp
*
* Created: 02.07.2020 17:56:12
* Author: dominik hellhake
*/
#include "BLELink.h"
#include "..\Peripheral\CC41A\CC41A.h"
#include "..\Peripheral\BNO055\BNO055.h"

BLELink BLE;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT BLELink::Run(uint32_t timeStamp)
{
	if (this->ReceiveBuffer[0] != 0x00)
	{
		switch (((BLE_CMD)this->ReceiveBuffer[0]))
		{
			case BLE_CMD::REQUEST_IMU_ValSet:
				this->ResponseIMUValSet();
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
BLELink::BLELink()
{
} //BLELink

void BLELink::ResponseIMUValSet()
{
	BLE_IMU_ValSet_1 valSet;
	
	valSet.Avl_AccX = IMU.Avl_AccX;
	valSet.Avl_AccY = IMU.Avl_AccY;
	valSet.Avl_AccZ = IMU.Avl_AccZ;
	valSet.Avl_GyroX = IMU.Avl_GyroX;
	valSet.Avl_GyroY = IMU.Avl_GyroY;
	valSet.Avl_GyroZ = IMU.Avl_GyroZ;
	valSet.Avl_Roll = IMU.Avl_Roll;
	valSet.Avl_Pitch = IMU.Avl_Pitch;
	valSet.Avl_Yaw = IMU.Avl_Yaw;
	
	BLEPort.WriteMessage(BLE_CMD::RESPONSE_IMU_ValSet, sizeof(BLE_IMU_ValSet_1), (uint8_t*)&valSet);
}