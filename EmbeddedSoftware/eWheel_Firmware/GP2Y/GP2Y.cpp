/* 
* GP2Y.cpp
*
* Created: 15.07.2018 12:49:11
* Author: dominik hellhake
*/

#include "GP2Y.h"

uint16_t GP2Y::Characteristics[CHARACTERISTIC_SIZE] = {
	2800, 2400,
	2000, 1775,
	1550, 1400,
	1250, 1150,
	1050, 950,
	850, 800,
	750, 700,
	650, 600,
	550, 525,
	500, 475,
	450, 425,
	400, 375,
	350, 325,
	300, 285,
	270, 260,
	250, 230
};

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT GP2Y::Run()
{
	float tmpRaw = 0;
	for (uint8_t x = 0; x < RAW_BUFFER_SIZE; x++)
		tmpRaw += this->Distance_RAW[x];
	
	tmpRaw /= RAW_BUFFER_SIZE;
	
	for (uint8_t x = 0; x < CHARACTERISTIC_SIZE; x++)
	{
		if (tmpRaw > GP2Y::Characteristics[x])
		{
			this->Distance = x + 4 - 1;
			break;
		}
	}
	
	this->UpdateReceived = false;
	return RUN_RESULT::SUCCESS;
}

void GP2Y::Propagate()
{
	if (this->OLED != NULL)
		this->OLED->SetRow(this->Distance, 1);
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
GP2Y::GP2Y()
{
	for (uint8_t x = 0; x < RAW_BUFFER_SIZE; x++)
		this->Distance_RAW[x] = x;
} //GP2Y