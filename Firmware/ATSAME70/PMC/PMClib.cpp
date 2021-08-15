/* 
* PMClib.cpp
*
* Created: 29.04.2020 21:04:23
* Author: dominik hellhake
*/
#include "PMClib.h"

void InitPMC()
{
	/* --- PLLA --- */
	// 12Mhz MAINCLK Input * 25 MULA / 1 DIVA => 300Mhz
	PMC->CKGR_PLLAR = CKGR_PLLAR_ONE | CKGR_PLLAR_MULA(25 - 1) | CKGR_PLLAR_DIVA(1) | CKGR_PLLAR_PLLACOUNT(0x3fU);;
	while (!(PMC->PMC_SR & PMC_SR_LOCKA));
		
	/* --- MCK --- */
	/* Set MCK Divider */
	PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_MDIV_Msk)) | PMC_MCKR_MDIV_PCK_DIV2;
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY));
	
	/* Set MCK Prescaler */
	PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_PRES_Msk)) | PMC_MCKR_PRES_CLK_1;
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY));
	
	/* Switch MCK to PLLA */
	PMC->PMC_MCKR = (PMC->PMC_MCKR & (~PMC_MCKR_CSS_Msk)) | PMC_MCKR_CSS_PLLA_CLK;
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY));	
}