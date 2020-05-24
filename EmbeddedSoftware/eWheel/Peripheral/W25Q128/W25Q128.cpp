/* 
* W25Q128.cpp
*
* Created: 24.05.2020 13:20:15
* Author: dominik hellhake
*/
#include "W25Q128.h"
#include "..\SAM4S\SPI\SPIlib.h"

W25Q128 DataFlash;


uint8_t W25Q128::ReadStatus()
{	
	uint8_t data [2] { 0x05, 0x00 };
	
	SPI_TransferData(SPI_PCS_PER_W25Q, data, 2);
	
	return data[1];
}

void W25Q128::ReadJEDEC()
{
	uint8_t data [4] { 0x9F, 0x00, 0x00, 0x00 };
	
	SPI_TransferData(SPI_PCS_PER_W25Q, data, 4);
	
	data[0]++;
}