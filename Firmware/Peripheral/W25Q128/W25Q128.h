/* 
* W25Q128.h
*
* Created: 24.05.2020 13:20:15
* Author: dominik hellhake
*/
#ifndef __W25Q128_H__
#define __W25Q128_H__

#include "sam.h"

#define W25Q_SPI			SPI

#define W25Q_PAGE_SIZE		256
#define W25Q_PAGE_COUNT		65536
#define W25Q_SECTOR_COUNT	4096

typedef enum W25Q_RESULT
{
	SUCCESS,
	BUSY
} W25Q_RESULT;

typedef struct W25Q_PAGE
{
	uint16_t Index;
	uint8_t data[W25Q_PAGE_SIZE];	
} W25Q_PAGE;

class W25Q128
{
	private:
		void WriteEnable();
		
	public:
		uint8_t ReadStatus();
		
		W25Q_RESULT SectorErase(uint16_t index);
		W25Q_RESULT PageRead(uint16_t index, uint8_t *data);	
		W25Q_RESULT PageProgram(uint16_t index, uint8_t *data);
		
		void ReadJEDEC();
}; //W25Q128

extern W25Q128 DataFlash;

#endif //__W25Q128_H__
