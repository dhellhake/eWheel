/* 
* HSMCIlib.cpp
*
* Created: 08.05.2021 11:58:39
* Author: dominik hellhake
*/
#include "HSMCIlib.h"
#include "SDCardCMD.h"
#include "SDCardReg.h"

void InitHSMCI()
{	
	PMC->PMC_PCER0 =		1 << ID_HSMCI;
	
	HSMCI->HSMCI_CR =		HSMCI_CR_MCIDIS;
	HSMCI->HSMCI_IDR =		~0;
	
	HSMCI->HSMCI_DTOR =		HSMCI_DTOR_DTOMUL_1048576 | HSMCI_DTOR_DTOCYC(2);
	HSMCI->HSMCI_CSTOR =	HSMCI_CSTOR_CSTOMUL_1048576 | HSMCI_CSTOR_CSTOCYC(2);
	HSMCI->HSMCI_CFG =		HSMCI_CFG_FIFOMODE | HSMCI_CFG_FERRCTRL;
	
	HSMCI->HSMCI_MR =		HSMCI_MR_CLKDIV(186) | HSMCI_MR_WRPROOF | HSMCI_MR_RDPROOF;
	
	HSMCI->HSMCI_CR =		HSMCI_CR_MCIEN;
	
	NVIC_DisableIRQ(HSMCI_IRQn);
	NVIC_ClearPendingIRQ(HSMCI_IRQn);
	NVIC_SetPriority(HSMCI_IRQn, 0);
	NVIC_EnableIRQ(HSMCI_IRQn);
}

uint32_t InitSDCard()
{
	uint32_t rca = 0x00;
	HSMCI->HSMCI_MR =		HSMCI_MR_CLKDIV(195) | HSMCI_MR_CLKODD;
	HSMCI->HSMCI_SDCR =		HSMCI_SDCR_SDCBUS_1;
	HSMCI->HSMCI_BLKR = 0;
	
	// CMD: INIT
	if (SetCMD(SD_CMD_INIT, 0) != 0)
		return 0;
	
	// CMD: GO_IDLE_STATE
	if (SetCMD(SD_CMD0, 0) != 0)
		return 0;
	
	// CMD: SEND_IF_COND
	do
	{
		if (SetCMD(SD_CMD8, 0x1aa) != 0)
			return 0;
		rca = HSMCI->HSMCI_RSPR[0];
	} while((HSMCI->HSMCI_RSPR[0] & 0xfff) != 0x1aa);
	
	// ACMD: SD_SEND_OP_COND
	do
	{
		if (SetCMD(SD_CMD55, 0) != 0)
			return 0;
		if ((SetCMD(SD_ACMD41, SD_ACMD41_HCS | SD_MMC_VOLTAGE_SUPPORT) & ~HSMCI_SR_RCRCE) != 0)
			return 0;
		
	} while (!(HSMCI->HSMCI_RSPR[0] & (1lu << 31)));
	
	// CMD: ALL_SEND_CID
	if (SetCMD(SD_CMD2, 0) != 0)
		return 0;
	
	// CMD: SEND_RELATIVE_ADDR
	if (SetCMD(SD_CMD3, 0) != 0)
		return 0;
	rca = (HSMCI->HSMCI_RSPR[0] >> 16) & 0xffff;
	
	// CMD: SELECT/DESELECT_CARD
	if (SetCMD(SD_CMD7, rca << 16) != 0)
		return 0;
	
	if (1)
	{
		// ACMD: SET_BUS_WIDTH
		if (SetCMD(SD_CMD55, rca << 16) != 0)
			return 0;
		if (SetCMD(SD_ACMD6, 2) != 0)
			return 0;
	}
	
	HSMCI->HSMCI_MR = HSMCI_MR_CLKDIV(4);
	HSMCI->HSMCI_SDCR = HSMCI_SDCR_SDCBUS_4;	
	HSMCI->HSMCI_BLKR = HSMCI_BLKR_BLKLEN(512) | HSMCI_BLKR_BCNT(1);
	
	// CMD: SET_BLOCKLEN
	if (SetCMD(SD_CMD16, 512) != 0)
		return 0;
	
	return rca;
}




uint32_t ReadSector(uint32_t rca, uint32_t lba, uint32_t* rxd)
{	
	// CMD: SEND_STATUS
	do
	{
		if (SetCMD(SD_CMD13, rca << 16) != 0)
			return 0;
	} while ((HSMCI->HSMCI_RSPR[0] & CARD_STATUS_READY_FOR_DATA) == 0);
		
	// CMD: READ_SINGLE_BLOCK
	if (SetCMD(SD_CMD17, lba) != 0)
		return 0;
	if (HSMCI->HSMCI_RSPR[0] & CARD_STATUS_ERR_RD_WR)
		return 0;
	
	for (uint32_t x = 0; x < 128; x++)
	{
		while ((HSMCI->HSMCI_SR & HSMCI_SR_RXRDY) == 0);
		rxd[x] = HSMCI->HSMCI_RDR;
	}
	
	return 1;
}

uint32_t WriteSector(uint32_t rca, uint32_t lba, uint32_t* data)
{
	// CMD: SEND_STATUS
	do
	{
		if (SetCMD(SD_CMD13, rca << 16) != 0)
			return 0;
	} while ((HSMCI->HSMCI_RSPR[0] & CARD_STATUS_READY_FOR_DATA) == 0);
	
	// CMD: WRITE_BLOCK
	if (SetCMD(SD_CMD24, lba) != 0)
		return 0;
	if (HSMCI->HSMCI_RSPR[0] & CARD_STATUS_ERR_RD_WR)
		return 0;
	
	for (uint32_t x = 0; x < 128; x++)
	{
		while ((HSMCI->HSMCI_SR & HSMCI_SR_TXRDY) == 0);
		HSMCI->HSMCI_TDR = data[x];
	}
	
	return 1;
}

uint32_t WriteMultipleSector(uint32_t rca, uint32_t lba, uint32_t* data, uint16_t blkcnt)
{
	
	HSMCI->HSMCI_BLKR = HSMCI_BLKR_BLKLEN(512) | HSMCI_BLKR_BCNT(blkcnt);
	// CMD: SEND_STATUS
	do
	{
		if (SetCMD(SD_CMD13, rca << 16) != 0)
			return 0;
	} while ((HSMCI->HSMCI_RSPR[0] & CARD_STATUS_READY_FOR_DATA) == 0);
	
	// CMD: WRITE_BLOCK
	if (SetCMD(SD_CMD25, lba) != 0)
		return 0;
	if (HSMCI->HSMCI_RSPR[0] & CARD_STATUS_ERR_RD_WR)
		return 0;
	
	for (uint32_t x = 0; x < (128 * blkcnt); x++)
	{
		while ((HSMCI->HSMCI_SR & HSMCI_SR_TXRDY) == 0);
		HSMCI->HSMCI_TDR = data[x];
	}
	
	while ((HSMCI->HSMCI_SR & HSMCI_SR_FIFOEMPTY) == 0);
	while ((HSMCI->HSMCI_SR & HSMCI_SR_TXRDY) == 0);
	SetCMD(SD_CMD12, 0);
		
	return 1;
}

void HSMCI_Handler(void)
{
	uint32_t status = HSMCI->HSMCI_SR;
	if (status & HSMCI_SR_CMDRDY & HSMCI->HSMCI_IMR)
	{
		HSMCI->HSMCI_IDR = HSMCI_IDR_CMDRDY;
	}
}
