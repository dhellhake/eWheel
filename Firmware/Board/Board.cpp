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
		
	return result;
}

RUN_RESULT Board::Setup(uint32_t timeStamp)
{
	return RUN_RESULT::SUCCESS;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
