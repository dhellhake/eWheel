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
		
	#define SPI_PCS_PER_LSM9DS1		0
	
	void InitSPI();
	
	inline void SPI_SelectPeripheral(uint8_t per_id)
	{		
		SPI->SPI_MR &= (~SPI_MR_PCS_Msk);
		SPI->SPI_MR |= SPI_MR_PCS(per_id);
	}
	
	inline void SPI_TransferData(uint8_t* data, uint16_t length)
	{						
		//Indicate multiple byte transfer
		SPI->SPI_CSR[0] |= SPI_CSR_CSAAT;
		
		for (uint16_t x = 0; x < length; x++)
		{
			SPI->SPI_TDR = data[x];
			while ((SPI->SPI_SR & SPI_SR_RDRF) == 0);
			data[x] = SPI->SPI_RDR;
		}
		
		//Idicate last transfer
		SPI->SPI_CR = SPI_CR_LASTXFER;
	}

#ifdef __cplusplus
}
#endif

#endif //__SPILIB_H__
