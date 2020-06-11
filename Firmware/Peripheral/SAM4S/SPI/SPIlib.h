/* 
* SPIlib.h
*
* Created: 13.05.2020 21:00:51
* Author: dominik hellhake
*/
#ifndef __SPILIB_H__
#define __SPILIB_H__


#ifdef __cplusplus
extern "C" {
#endif
	#include "sam.h"
		
	void InitSPI();
	
	typedef enum SPI_PCS_PER
	{
		W25Q = 0
	}SPI_PCS_PER;
	
	inline void SPI_Select(SPI_PCS_PER per)
	{
		SPI->SPI_CSR[(uint8_t)per] |= SPI_CSR_CSAAT;
	}
	
	inline void SPI_Finish()
	{	
		SPI->SPI_CR = SPI_CR_LASTXFER;	
	}
	
	inline void SPI_TransferData(uint8_t *data, uint16_t length)
	{	
		for (uint16_t x = 0; x < length; x++)
		{
			SPI->SPI_TDR = data[x];
			while ((SPI->SPI_SR & SPI_SR_RDRF) == 0);
			data[x] = SPI->SPI_RDR;
		}
	}

#ifdef __cplusplus
}
#endif

#endif //__SPILIB_H__
