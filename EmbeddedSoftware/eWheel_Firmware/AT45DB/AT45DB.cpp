/* 
* AT45DB.cpp
*
* Created: 25.02.2019 20:08:29
* Author: Dominik Hellhake
*/


#include "AT45DB.h"

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
RUN_RESULT AT45DB::Run()
{	
	uint8_t tmp[6];
	
	this->ReadBytes(0xD7, tmp, 1);
	
	return RUN_RESULT::ERROR;
}

void AT45DB::Propagate()
{
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
AT45DB::AT45DB()
{
	//Init SPI-SERCOM interface
	SPIPort::InitSERCOM0();
	
	//Configure CS-Pin as Output
	PORT->Group[0].DIRSET.reg = PORT_PA08;
	PORT->Group[0].OUTSET.reg = PORT_PA08;
}

uint8_t AT45DB::ReadBytes(uint8_t address, uint8_t *dest, uint8_t count)
{
	//CS Low
	PORT->Group[0].OUTCLR.reg = PORT_PA08;
	
	SPIPort::SERCOM0_TransmitByte(address);
	for (uint8_t x = 0; x < count; x++)
	dest[x] = SPIPort::SERCOM0_TransmitByte(0x00);
	
	//CS High
	PORT->Group[0].OUTSET.reg = PORT_PA08;
	
	return 1;
}