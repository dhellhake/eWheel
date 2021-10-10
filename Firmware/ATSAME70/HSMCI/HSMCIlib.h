/* 
* HSMCIlib.h
*
* Created: 08.05.2021 11:58:39
* Author: dominik hellhake
*/
#ifndef __HSMCILIB_H__
#define __HSMCILIB_H__

#define HSMCI_SR_ERROR (HSMCI_SR_RINDE | HSMCI_SR_RDIRE | HSMCI_SR_RCRCE | HSMCI_SR_RENDE | HSMCI_SR_RTOE | HSMCI_SR_DCRCE | HSMCI_SR_DTOE | HSMCI_SR_CSTOE | HSMCI_SR_BLKOVRE | HSMCI_SR_ACKRCVE | HSMCI_SR_OVRE | HSMCI_SR_UNRE)


#ifdef __cplusplus
	extern "C" {
#endif

	#include "sam.h"
	

	void InitHSMCI();
	uint32_t InitSDCard();
	
	inline uint32_t SetCMD(uint32_t cmd, uint32_t arg)
	{
		HSMCI->HSMCI_ARGR = arg;
		HSMCI->HSMCI_CMDR = cmd;
		
		while ((HSMCI->HSMCI_SR & HSMCI_SR_CMDRDY) == 0);
		
		return HSMCI->HSMCI_SR & HSMCI_SR_ERROR;
	}
	
	uint32_t ReadSector(uint32_t rca, uint32_t lba, uint32_t* rxd);
	uint32_t WriteSector(uint32_t rca, uint32_t lba, uint32_t* data);
	uint32_t WriteMultipleSector(uint32_t rca, uint32_t lba, uint32_t* data, uint16_t blkcnt);

#ifdef __cplusplus
	}
#endif

#endif //__HSMCILIB_H__
