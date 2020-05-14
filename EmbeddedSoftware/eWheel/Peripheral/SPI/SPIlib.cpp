/* 
* SPIlib.cpp
*
* Created: 13.05.2020 21:00:51
* Author: dominik hellhake
*/
#include "SPIlib.h"

void InitSPI()
{
	//Disable SPI
	SPI->SPI_CR = SPI_CR_SPIDIS;
	//Reset SPI
	SPI->SPI_CR = SPI_CR_SWRST;
	//Idicate last transfer
	SPI->SPI_CR = SPI_CR_LASTXFER;
	//Set Master-Mode
	SPI->SPI_MR |= SPI_MR_MSTR;
	//Disable Mode Fault Detection.
	SPI->SPI_MR |= SPI_MR_MODFDIS;
	//Deassert Chip-Select
	SPI->SPI_MR &= (~SPI_MR_PCS_Msk);
	SPI->SPI_MR |= SPI_MR_PCS(0x0);
	
	/* SPI to LSM9DS1 */
	// Set SPI-Mode 1
	SPI->SPI_CSR[0] &= ~(SPI_CSR_CPOL);
	SPI->SPI_CSR[0] |= (SPI_CSR_NCPHA);
	// Set 8 bits per transfer
	SPI->SPI_CSR[0] |= SPI_CSR_BITS_8_BIT;
	// Set bautrate to 6 Mhz
	SPI->SPI_CSR[0] &= (~SPI_CSR_SCBR_Msk);
	SPI->SPI_CSR[0] |= SPI_CSR_SCBR(20);
	// Set Chip-Select setup and multiple transfer delay
	SPI->SPI_CSR[0] &= ~(SPI_CSR_DLYBS_Msk | SPI_CSR_DLYBCT_Msk);
	SPI->SPI_CSR[0] |= SPI_CSR_DLYBS(0x0) | SPI_CSR_DLYBCT(0);
	
	// Enable SPI
	SPI->SPI_CR = SPI_CR_SPIEN;	
}