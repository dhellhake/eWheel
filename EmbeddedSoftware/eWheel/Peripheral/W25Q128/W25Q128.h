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

class W25Q128
{
	public:
		uint8_t ReadStatus();
		void ReadJEDEC();
}; //W25Q128

extern W25Q128 DataFlash;

#endif //__W25Q128_H__
