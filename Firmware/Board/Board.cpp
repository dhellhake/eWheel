/* 
* Board.cpp
*
* Created: 22.05.2020 13:43:45
* Author: dominik hellhake
*/
#include <math.h>
#include "Board.h"
#include "..\Peripheral\BNO055\BNO055.h"
#include "..\Peripheral\ADS1118\ADS1118.h"


#include "..\Peripheral/W25Q128/W25Q128.h"

Board Chassis;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT Board::Run(uint32_t timeStamp)
{
	RUN_RESULT result = RUN_RESULT::IDLE;
	
	if (timeStamp - this->IMU_Update_Tstmp >= 20)
	{		
		IMU.Update();
		this->IMU_Update_Tstmp = timeStamp;
		result = RUN_RESULT::SUCCESS;
	}
	
	if (timeStamp - this->ADS_Update_Tstmp >= 10)
	{
		//DataFlash.ReadJEDEC();
		ADS.Update();
		
		this->ADS_Update_Tstmp = timeStamp;		
		result = RUN_RESULT::SUCCESS;
	}
		
	return result;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
