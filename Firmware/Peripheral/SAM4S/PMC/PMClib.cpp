/* 
* PMClib.cpp
*
* Created: 29.04.2020 21:04:23
* Author: dominik hellhake
*/
#include "PMClib.h"

void InitPMC()
{
	// Set Flash Wait States (FWS = 5 for 120Mhz)
	EFC0->EEFC_FMR = EEFC_FMR_FWS(5) | EEFC_FMR_CLOE;
	
	// Enable external oscillator
	PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTST(0xfful) | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN;
	while ((PMC->PMC_SR & PMC_SR_MOSCXTS) != PMC_SR_MOSCXTS);
	// Switch to external oscillator
	PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTST(0xfful) | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN | CKGR_MOR_MOSCSEL;
	while ((PMC->PMC_SR & PMC_SR_MOSCSELS) != PMC_SR_MOSCSELS);

	// Set PLLA Clock to 10/1 * MainClock(12Mhz)
	PMC->CKGR_PLLAR = CKGR_PLLAR_ONE | CKGR_PLLAR_MULA(10-1) | CKGR_PLLAR_DIVA(1) | CKGR_PLLAR_PLLACOUNT(0x3fU);
	while (!(PMC->PMC_SR & PMC_SR_LOCKA));	
	// Select PLLA as source for Processor-Clock
	PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_CSS_Msk)) | PMC_MCKR_CSS_PLLA_CLK;
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY));	
}
