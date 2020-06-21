/* 
* W25Q128.cpp
*
* Created: 24.05.2020 13:20:15
* Author: dominik hellhake
*/
#include "W25Q128.h"
#include "..\SAM4S\SPI\SPIlib.h"

W25Q128 DataFlash;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT W25Q128::Run(uint32_t timeStamp)
{
	RUN_RESULT result = RUN_RESULT::IDLE;
	
	return result;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
W25Q_RESULT W25Q128::PageRead(uint16_t index, uint8_t *data)
{
	uint8_t status = this->ReadStatus();
	if (status != 0x00)
		return W25Q_RESULT::BUSY;
	
	uint8_t data_pre [4] {
		0x03,
		(uint8_t)(index >> 8),
		(uint8_t)(index & 0xFF),
		0x00
	};
		
	SPI_Select(SPI_PCS_PER::W25Q);
	SPI_TransferData(data_pre, 4);
	SPI_TransferData(data, 256);
	SPI_Finish();
	
	return W25Q_RESULT::SUCCESS;
}

W25Q_RESULT W25Q128::PageProgram(uint16_t index, uint8_t *data)
{
	uint8_t status = this->ReadStatus();
	if (status != 0x00)
		return W25Q_RESULT::BUSY;
	
	this->WriteEnable();
	
	uint8_t data_pre [4] {
		0x02,
		(uint8_t)(index >> 8),
		(uint8_t)(index & 0xFF),
		0x00
	};
		
	SPI_Select(SPI_PCS_PER::W25Q);	
	SPI_TransferData(data_pre, 4);
	SPI_TransferData(data, 256);
	SPI_Finish();
	
	return W25Q_RESULT::SUCCESS;	
}


W25Q_RESULT W25Q128::SectorErase(uint16_t index)
{
	uint8_t status = this->ReadStatus();
	if (status != 0x00)
		return W25Q_RESULT::BUSY;	
	
	this->WriteEnable();
	
	index = (index << 4);	
	uint8_t data [4] { 
		0x20,
		(uint8_t)(index >> 8),
		(uint8_t)(index & 0xFF),
		0x00
	};
	
	SPI_Select(SPI_PCS_PER::W25Q);
	SPI_TransferData(data, 4);
	SPI_Finish();
	
	return W25Q_RESULT::SUCCESS;	
}

uint8_t W25Q128::ReadStatus()
{
	uint8_t data [2] { 0x05, 0x00 };
		
	SPI_Select(SPI_PCS_PER::W25Q);
	SPI_TransferData(data, 2);
	SPI_Finish();
	
	return data[1];
}

void W25Q128::WriteEnable()
{
	uint8_t data [1] { 0x06 };
		
	SPI_Select(SPI_PCS_PER::W25Q);
	SPI_TransferData(data, 1);
	SPI_Finish();
}

void W25Q128::ReadJEDEC()
{
	uint8_t data [4] { 0x9F, 0x00, 0x00, 0x00 };
	
	SPI_Select(SPI_PCS_PER::W25Q);
	SPI_TransferData(data, 4);
	SPI_Finish();
}